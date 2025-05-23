/*
 * OpenSSL API (BIO) - Harbour interface.
 *
 * Copyright 2009-2016 Viktor Szakats (vszakats.net/harbour)
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2, or (at your option)
 * any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; see the file LICENSE.txt.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor,
 * Boston, MA 02110-1301 USA (or visit https://www.gnu.org/licenses/).
 *
 * As a special exception, the Harbour Project gives permission for
 * additional uses of the text contained in its release of Harbour.
 *
 * The exception is that, if you link the Harbour libraries with other
 * files to produce an executable, this does not by itself cause the
 * resulting executable to be covered by the GNU General Public License.
 * Your use of that executable is in no way restricted on account of
 * linking the Harbour library code into it.
 *
 * This exception does not however invalidate any other reasons why
 * the executable file might be covered by the GNU General Public License.
 *
 * This exception applies only to the code released by the Harbour
 * Project under the name Harbour.  If you copy code from other
 * Harbour Project or Free Software Foundation releases into a copy of
 * Harbour, as the General Public License permits, the exception does
 * not apply to the code that you add in this way.  To avoid misleading
 * anyone as to the status of such modified files, you must delete
 * this exception notice from them.
 *
 * If you write modifications of your own for Harbour, it is your choice
 * whether to permit this exception to apply to your modifications.
 * If you do not wish that, delete this exception notice.
 *
 */

#include "hbssl.h"

#include "hbapiitm.h"

/* */

typedef struct
{
   BIO *    bio;
   void *   hStrRef;
} HB_BIO, * PHB_BIO;

static PHB_BIO PHB_BIO_create( BIO * bio, void * hStrRef )
{
   PHB_BIO hb_bio = ( PHB_BIO ) hb_xgrab( sizeof( HB_BIO ) );

   hb_bio->bio       = bio;
   hb_bio->hStrRef   = hStrRef;

   return hb_bio;
}

static void PHB_BIO_free( PHB_BIO hb_bio )
{
   if( hb_bio->bio )
      BIO_free( hb_bio->bio );
   if( hb_bio->hStrRef )
      hb_itemFreeCRef( hb_bio->hStrRef );

   hb_xfree( hb_bio );
}

/* HB_BIO GC handler */

/* BIO destructor, it's executed automatically */
static HB_GARBAGE_FUNC( HB_BIO_Destructor )
{
   /* Retrieve image pointer holder */
   HB_BIO ** ptr = ( HB_BIO ** ) Cargo;

   /* Check if pointer is not NULL to avoid multiple freeing */
   if( *ptr )
   {
      PHB_BIO_free( *ptr );

      /* set pointer to NULL to avoid multiple freeing */
      *ptr = NULL;
   }
}

static const HB_GC_FUNCS s_gcBIOFuncs =
{
   HB_BIO_Destructor,
   hb_gcDummyMark
};

BIO * hb_BIO_par( int iParam )
{
   HB_BIO ** ptr = ( HB_BIO ** ) hb_parptrGC( &s_gcBIOFuncs, iParam );

   return ptr ? ( *ptr )->bio : NULL;
}

HB_BOOL hb_BIO_is( int iParam )
{
   HB_BIO ** ptr = ( HB_BIO ** ) hb_parptrGC( &s_gcBIOFuncs, iParam );

   return ptr && ( *ptr )->bio;
}

static void hb_BIO_ret( BIO * bio, void * hStrRef )
{
   HB_BIO ** ptr = ( HB_BIO ** ) hb_gcAllocate( sizeof( HB_BIO * ), &s_gcBIOFuncs );

   *ptr = PHB_BIO_create( bio, hStrRef );

   hb_retptrGC( ( void * ) ptr );
}

/* */

static HB_BOOL hb_BIO_METHOD_is( int iParam )
{
   return HB_ISCHAR( iParam );
}

#if OPENSSL_VERSION_NUMBER >= 0x10100000L
static const BIO_METHOD * hb_BIO_METHOD_par( int iParam )
#else
static BIO_METHOD * hb_BIO_METHOD_par( int iParam )
#endif
{
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
   const BIO_METHOD * p;
#else
   BIO_METHOD * p;
#endif

   switch( hb_parni( iParam ) )
   {
      case HB_BIO_METHOD_S_NULL:        p = BIO_s_null();       break;
#ifndef OPENSSL_NO_FP_API
      case HB_BIO_METHOD_S_FILE:        p = BIO_s_file();       break;
#endif
      case HB_BIO_METHOD_S_MEM:         p = BIO_s_mem();        break;
#ifndef OPENSSL_NO_SOCK
      case HB_BIO_METHOD_S_SOCKET:      p = BIO_s_socket();     break;
      case HB_BIO_METHOD_S_CONNECT:     p = BIO_s_connect();    break;
      case HB_BIO_METHOD_S_ACCEPT:      p = BIO_s_accept();     break;
#endif
      case HB_BIO_METHOD_S_FD:          p = BIO_s_fd();         break;
#if 0 /* BIO_s_log() isn't exported via implibs on Windows at version 0.9.8k. [vszakats] */
#ifndef OPENSSL_SYS_OS2
      case HB_BIO_METHOD_S_LOG:         p = BIO_s_log();        break;
#endif
#endif
      case HB_BIO_METHOD_S_BIO:         p = BIO_s_bio();        break;
#ifndef OPENSSL_NO_DGRAM
      case HB_BIO_METHOD_S_DATAGRAM:    p = BIO_s_datagram();   break;
#endif
      case HB_BIO_METHOD_F_NULL:        p = BIO_f_null();       break;
      case HB_BIO_METHOD_F_BUFFER:      p = BIO_f_buffer();     break;
#ifdef OPENSSL_SYS_VMS
      case HB_BIO_METHOD_F_LINEBUFFER:  p = BIO_f_linebuffer(); break;
#endif
      case HB_BIO_METHOD_F_NBIO_TEST:   p = BIO_f_nbio_test();  break;
      default:                          p = NULL;
   }

   return p;
}

#if 0
/* NOTE: Unused yet. Commented to avoid warning */
static int hb_BIO_METHOD_ptr_to_id( const BIO_METHOD * p )
{
   int n;

   if(      p == BIO_s_null()       ) n = HB_BIO_METHOD_S_NULL;
#ifndef OPENSSL_NO_FP_API
   else if( p == BIO_s_file()       ) n = HB_BIO_METHOD_S_FILE;
#endif
   else if( p == BIO_s_mem()        ) n = HB_BIO_METHOD_S_MEM;
#ifndef OPENSSL_NO_SOCK
   else if( p == BIO_s_socket()     ) n = HB_BIO_METHOD_S_SOCKET;
   else if( p == BIO_s_connect()    ) n = HB_BIO_METHOD_S_CONNECT;
   else if( p == BIO_s_accept()     ) n = HB_BIO_METHOD_S_ACCEPT;
#endif
   else if( p == BIO_s_fd()         ) n = HB_BIO_METHOD_S_FD;
#if 0 /* BIO_s_log() isn't exported via implibs on Windows at version 0.9.8k. [vszakats] */
#ifndef OPENSSL_SYS_OS2
   else if( p == BIO_s_log()        ) n = HB_BIO_METHOD_S_LOG;
#endif
#endif
   else if( p == BIO_s_bio()        ) n = HB_BIO_METHOD_S_BIO;
#ifndef OPENSSL_NO_DGRAM
   else if( p == BIO_s_datagram()   ) n = HB_BIO_METHOD_S_DATAGRAM;
#endif
   else if( p == BIO_f_null()       ) n = HB_BIO_METHOD_F_NULL;
   else if( p == BIO_f_buffer()     ) n = HB_BIO_METHOD_F_BUFFER;
#ifdef OPENSSL_SYS_VMS
   else if( p == BIO_f_linebuffer() ) n = HB_BIO_METHOD_F_LINEBUFFER;
#endif
   else if( p == BIO_f_nbio_test()  ) n = HB_BIO_METHOD_F_NBIO_TEST;
   else                               n = HB_BIO_METHOD_UNSUPPORTED;

   return n;
}
#endif

HB_FUNC( BIO_NEW )
{
   if( hb_BIO_METHOD_is( 1 ) )
      hb_BIO_ret( BIO_new( hb_BIO_METHOD_par( 1 ) ), NULL );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio && hb_BIO_METHOD_is( 2 ) )
#if OPENSSL_VERSION_NUMBER < 0x10100000L || \
    ( defined( LIBRESSL_VERSION_NUMBER ) && LIBRESSL_VERSION_NUMBER < 0x30900000L )
      hb_retni( BIO_set( bio, hb_BIO_METHOD_par( 2 ) ) );
#else
      hb_retni( 0 );
#endif
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_CLEAR_FLAGS )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      BIO_clear_flags( bio, hb_parni( 2 ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_FLAGS )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      BIO_set_flags( bio, hb_parni( 2 ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_FLAGS )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_get_flags( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_TEST_FLAGS )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
#if OPENSSL_VERSION_NUMBER >= 0x00908050L && ! defined( HB_OPENSSL_OLD_OSX_ )
      hb_retni( BIO_test_flags( bio, hb_parni( 2 ) ) );
#else
      hb_retni( 0 );
#endif
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_FD )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_set_fd( bio, hb_parnl( 2 ), hb_parnidef( 3, BIO_NOCLOSE ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_FD )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retnl( BIO_get_fd( bio, NULL ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_RETRY_REASON )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_get_retry_reason( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_RETRY_SPECIAL )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      BIO_set_retry_special( bio );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_RETRY_READ )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      BIO_set_retry_read( bio );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_RETRY_WRITE )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      BIO_set_retry_write( bio );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SHOULD_READ )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_should_read( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SHOULD_WRITE )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_should_write( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SHOULD_IO_SPECIAL )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_should_io_special( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_RETRY_TYPE )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_retry_type( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SHOULD_RETRY )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_should_retry( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_CTRL_PENDING )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retnint( BIO_ctrl_pending( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_CTRL_WPENDING )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retnint( BIO_ctrl_wpending( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_FLUSH )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_flush( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SEEK )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retnl( BIO_seek( bio, hb_parnl( 2 ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_TELL )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retnl( BIO_tell( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_RESET )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_reset( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_EOF )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_eof( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_CLOSE )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_set_close( bio, hb_parnidef( 2, BIO_NOCLOSE ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_CLOSE )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_get_close( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_NEW_SOCKET )
{
   if( HB_ISNUM( 1 ) )
      hb_BIO_ret( BIO_new_socket( hb_parni( 1 ), hb_parnidef( 2, BIO_NOCLOSE ) ), NULL );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_NEW_DGRAM )
{
#ifndef OPENSSL_NO_DGRAM
   if( HB_ISNUM( 1 ) )
      hb_BIO_ret( BIO_new_dgram( hb_parni( 1 ), hb_parnidef( 2, BIO_NOCLOSE ) ), NULL );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#else
   hb_errRT_BASE( EG_NOFUNC, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#endif
}

HB_FUNC( BIO_NEW_FD )
{
   if( HB_ISNUM( 1 ) )
      hb_BIO_ret( BIO_new_fd( hb_parnl( 1 ), hb_parnidef( 2, BIO_NOCLOSE ) ), NULL );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_NEW_FILE )
{
   if( HB_ISCHAR( 1 ) )
      hb_BIO_ret( BIO_new_file( hb_parc( 1 ), hb_parcx( 2 ) ), NULL );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_NEW_MEM_BUF )
{
   PHB_ITEM pBuffer = hb_param( 1, HB_IT_STRING );

   if( pBuffer )
   {
      void * hStrRef;
      HB_SIZE nLen;
      const char * pszBuffer = hb_itemGetCRef( pBuffer, &hStrRef, &nLen );

      hb_BIO_ret( BIO_new_mem_buf( HB_UNCONST( pszBuffer ), ( int ) nLen ), hStrRef );
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_READ )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
   {
      int size = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );

      if( size > 0 )
      {
         char * buffer = ( char * ) hb_xgrab( size + 1 );

         hb_retni( size = BIO_read( bio, buffer, size ) );

         if( ! hb_storclen( buffer, size, 2 ) )
            hb_xfree( buffer );
      }
      else
      {
         hb_retni( 0 );
         hb_storc( NULL, 2 );
      }
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GETS )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
   {
      int size = HB_ISNUM( 3 ) ? hb_parni( 3 ) : ( int ) hb_parclen( 2 );

      if( size > 0 )
      {
         char * buffer = ( char * ) hb_xgrab( size + 1 );

         hb_retni( size = BIO_gets( bio, buffer, size ) );

         if( ! hb_storclen( buffer, size, 2 ) )
            hb_xfree( buffer );
      }
      else
      {
         hb_retni( 0 );
         hb_storc( NULL, 2 );
      }
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_WRITE )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
   {
      int size = ( int ) hb_parclen( 2 );

      if( HB_ISNUM( 3 ) )
      {
         int towrite = hb_parni( 3 );
         if( towrite >= 0 && towrite < size )
            size = towrite;
      }

      hb_retni( BIO_write( bio, hb_parcx( 2 ), size ) );
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_PUTS )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_puts( bio, hb_parcx( 2 ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_FREE )
{
   HB_BIO ** ptr = ( HB_BIO ** ) hb_parptrGC( &s_gcBIOFuncs, 1 );

   if( ptr )
   {
      int result = 0;

      if( *ptr )
      {
         PHB_BIO_free( *ptr );
         *ptr = NULL;
         result = 1;
      }
      hb_retni( result );
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC_TRANSLATE( BIO_VFREE, BIO_FREE )
HB_FUNC_TRANSLATE( BIO_FREE_ALL, BIO_FREE )  /* These wrappers don't allow to create chained BIOs, so this is valid. */

/* --- connect --- */

HB_FUNC( BIO_NEW_CONNECT )
{
   if( HB_ISCHAR( 1 ) )
#if OPENSSL_VERSION_NUMBER >= 0x10002000L && \
    ! defined( LIBRESSL_VERSION_NUMBER )
      hb_BIO_ret( BIO_new_connect( hb_parc( 1 ) ), NULL );
#else
      /* NOTE: Discarding 'const', OpenSSL will strdup() */
      hb_BIO_ret( BIO_new_connect( ( char * ) HB_UNCONST( hb_parc( 1 ) ) ), NULL );
#endif
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_NEW_ACCEPT )
{
   if( HB_ISCHAR( 1 ) )
#if OPENSSL_VERSION_NUMBER >= 0x10002000L && \
    ! defined( LIBRESSL_VERSION_NUMBER )
      hb_BIO_ret( BIO_new_accept( hb_parc( 1 ) ), NULL );
#else
      /* NOTE: Discarding 'const', OpenSSL will strdup() */
      hb_BIO_ret( BIO_new_accept( ( char * ) HB_UNCONST( hb_parc( 1 ) ) ), NULL );
#endif
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_CONN_HOSTNAME )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio && HB_ISCHAR( 2 ) )
      hb_retnl( BIO_set_conn_hostname( bio, HB_UNCONST( hb_parc( 2 ) ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_CONN_PORT )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio && HB_ISCHAR( 2 ) )
      hb_retnl( BIO_set_conn_port( bio, HB_UNCONST( hb_parc( 2 ) ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_CONN_INT_PORT )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio && HB_ISNUM( 2 ) )
   {
      int port = hb_parni( 2 );
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
      char szPort[ 12 ];
      hb_retnl( BIO_set_conn_port( bio, hb_numToStr( szPort, sizeof( szPort ), port ) ) );
#else
      hb_retnl( BIO_set_conn_int_port( bio, &port ) );
#endif
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_SET_CONN_IP )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio && HB_ISCHAR( 2 ) && hb_parclen( 2 ) == 4 )
   {
#if OPENSSL_VERSION_NUMBER >= 0x10100000L
      HB_SYMBOL_UNUSED( bio );  /* TODO: reimplement using BIO_set_conn_address() */
      hb_retnl( 0 );
#else
      if( hb_parclen( 2 ) == 4 )
         hb_retnl( BIO_set_conn_ip( bio, HB_UNCONST( hb_parc( 2 ) ) ) );
      else
         hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#endif
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_CONN_HOSTNAME )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retc( BIO_get_conn_hostname( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_CONN_PORT )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retc( BIO_get_conn_port( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_CONN_IP )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
   {
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL && \
    ! defined( LIBRESSL_VERSION_NUMBER )
      const BIO_ADDR * ba = BIO_get_conn_address( bio );
      char * pszAddr = ba ? BIO_ADDR_hostname_string( ba, 1 ) : NULL;

      hb_retc( pszAddr );
      if( pszAddr )
         OPENSSL_free( pszAddr );
#elif OPENSSL_VERSION_NUMBER >= 0x00906040L
      hb_retc( BIO_get_conn_ip( bio ) );
#else
      hb_retc( BIO_get_conn_ip( bio, 0 ) );
#endif
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_GET_CONN_INT_PORT )
{
#if OPENSSL_VERSION_NUMBER >= 0x10001000L  /* fixed here: https://rt.openssl.org/Ticket/Display.html?id=1989&user=guest&pass=guest */
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
   {
#if OPENSSL_VERSION_NUMBER == 0x1000206fL /* 1.0.2f */ || \
    OPENSSL_VERSION_NUMBER == 0x1000112fL /* 1.0.1r */
      /* Fix for header regression */
      hb_retnl( BIO_ctrl( bio, BIO_C_GET_CONNECT, 3, NULL ) );
#elif OPENSSL_VERSION_NUMBER >= 0x1010000fL && \
      ! defined( LIBRESSL_VERSION_NUMBER )
      const BIO_ADDR * ba = BIO_get_conn_address( bio );
      hb_retnl( ba ? hb_socketNToHS( BIO_ADDR_rawport( ba ) ) : 0 );
#else
      hb_retnl( BIO_get_conn_int_port( bio ) );
#endif
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#else
   hb_errRT_BASE( EG_UNSUPPORTED, 2001, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#endif
}

HB_FUNC( BIO_GET_CONN_ADDRESS )
{
#if OPENSSL_VERSION_NUMBER >= 0x1010000fL && \
    ! defined( LIBRESSL_VERSION_NUMBER )
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
   {
      const BIO_ADDR * ba = BIO_get_conn_address( bio );

      if( ba )
      {
         int family = BIO_ADDR_family( ba );
         char * pszAddr = BIO_ADDR_hostname_string( ba, 1 );

         hb_reta( family == HB_SOCKET_AF_LOCAL ? 2 : 3 );
         hb_storvni( family, -1, 1 );
         hb_storvc( pszAddr, -1, 2 );
         if( family != HB_SOCKET_AF_LOCAL )
            hb_storvni( hb_socketNToHS( BIO_ADDR_rawport( ba ) ), -1, 3 );
         if( pszAddr )
            OPENSSL_free( pszAddr );
      }
   }
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#else
   hb_errRT_BASE( EG_UNSUPPORTED, 2001, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
#endif
}

HB_FUNC( BIO_SET_NBIO )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retnl( BIO_set_nbio( bio, hb_parni( 2 ) ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( BIO_DO_CONNECT )
{
   BIO * bio = hb_BIO_par( 1 );

   if( bio )
      hb_retni( BIO_do_connect( bio ) );
   else
      hb_errRT_BASE( EG_ARG, 2010, NULL, HB_ERR_FUNCNAME, HB_ERR_ARGS_BASEPARAMS );
}

HB_FUNC( ERR_LOAD_BIO_STRINGS )
{
#if OPENSSL_VERSION_NUMBER < 0x30000000L
   ERR_load_BIO_strings();
#endif
}

#if 0

#define BIO_set_url( b, url )                 BIO_ctrl( b, BIO_C_SET_PROXY_PARAM, 0, ( char * ) ( url ) )
#define BIO_set_proxies( b, p )               BIO_ctrl( b, BIO_C_SET_PROXY_PARAM, 1, ( char * ) ( p ) )
/* BIO_set_nbio(b,n) */
#define BIO_set_filter_bio( b, s )            BIO_ctrl( b, BIO_C_SET_PROXY_PARAM, 2, ( char * ) ( s ) )
/* BIO *BIO_get_filter_bio(BIO *bio); */
#define BIO_set_proxy_cb( b, cb )             BIO_callback_ctrl( b, BIO_C_SET_PROXY_PARAM, 3, ( void *( *cb )( ) ) )
#define BIO_set_proxy_header( b, sk )         BIO_ctrl( b, BIO_C_SET_PROXY_PARAM, 4, ( char * ) sk )
#define BIO_set_no_connect_return( b, bool )  BIO_int_ctrl( b, BIO_C_SET_PROXY_PARAM, 5, bool )

#define BIO_get_proxy_header( b, skp )        BIO_ctrl( b, BIO_C_GET_PROXY_PARAM, 0, ( char * ) skp )
#define BIO_get_proxies( b, pxy_p )           BIO_ctrl( b, BIO_C_GET_PROXY_PARAM, 1, ( char * ) ( pxy_p ) )
#define BIO_get_url( b, url )                 BIO_ctrl( b, BIO_C_GET_PROXY_PARAM, 2, ( char * ) ( url ) )
#define BIO_get_no_connect_return( b )        BIO_ctrl( b, BIO_C_GET_PROXY_PARAM, 5, NULL )

#define BIO_set_fp( b, fp, c )                BIO_ctrl( b, BIO_C_SET_FILE_PTR, c, ( char * ) fp )
#define BIO_get_fp( b, fpp )                  BIO_ctrl( b, BIO_C_GET_FILE_PTR, 0, ( char * ) fpp )

int   BIO_indent( BIO * b, int indent, int max );
long  BIO_ctrl( BIO * bp, int cmd, long larg, void * parg );
long  BIO_callback_ctrl( BIO * b, int cmd, void ( * fp )( struct bio_st *, int, const char *, int, long, long ) );
char * BIO_ptr_ctrl( BIO * bp, int cmd, long larg );
long  BIO_int_ctrl( BIO * bp, int cmd, long larg, int iarg );
BIO * BIO_push( BIO * b, BIO * append );
BIO * BIO_pop( BIO * b );
BIO * BIO_find_type( BIO * b, int bio_type );
BIO * BIO_next( BIO * b );
BIO * BIO_get_retry_BIO( BIO * bio, int * reason );
BIO * BIO_dup_chain( BIO * in );

int BIO_nread0( BIO * bio, char ** buf );
int BIO_nread( BIO * bio, char ** buf, int num );
int BIO_nwrite0( BIO * bio, char ** buf );
int BIO_nwrite( BIO * bio, char ** buf, int num );

BIO_METHOD *   BIO_s_mem( void );

BIO_set_mem_eof_return( BIO * b, int v )
long BIO_get_mem_data( BIO * b, char ** pp )
BIO_set_mem_buf( BIO * b, BUF_MEM * bm, int c )
BIO_get_mem_ptr( BIO * b, BUF_MEM * *pp )

BIO * BIO_new_mem_buf( void * buf, int len );

#endif
