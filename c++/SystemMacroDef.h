/*!
*	\file	 SystemMacroDef.h
*	\brief	 系统调用宏定义声明头文件
*	\date	 2008-3-15
*	\author	 张瑾(zhangjin@software.ict.ac.cn)
*/
#ifndef H_SYSTEMMACRODEF_H
#define H_SYSTEMMACRODEF_H

#ifdef _WIN32

#include <windows.h>
#include <process.h>
#include <wincon.h> 
#include <nb30.h>
#include <conio.h>
#include <io.h>

// 锁资源宏定义
#define DEFINE_LOCKER(_locker)				HANDLE _locker
#define INITIALIZE_LOCKER(_locker)			_locker = CreateMutex(NULL, FALSE, NULL)
#define INVALID_LOCKER(_locker)				_locker == NULL
#define LOCK(_locker)						WaitForSingleObject(_locker, INFINITE)
#define UNLOCK(_locker)						ReleaseMutex(_locker)
#define DESTROY_LOCKER(_locker)				CloseHandle(_locker)

// 事件的宏定义
#define DEFINE_EVENT(_event)				HANDLE _event
#define INITIALIZE_EVENT(_event)			_event = CreateEvent(NULL, TRUE, FALSE, NULL)
#define INVALID_EVENT(_event)				_event == NULL
#define BROADCAST_EVENT(_event)				SetEvent(_event);
#define WAIT_EVENT(_event, _locker)			ResetEvent(_event);\
											ReleaseMutex(_locker);\
											WaitForSingleObject(_event, INFINITE);\
											WaitForSingleObject(_locker, INFINITE)
#define DESTROY_EVENT(_event)				CloseHandle(_event)

// 线程相关的宏定义
#define DEFINE_THREAD(_thread)				HANDLE _thread
#define BEGIN_THREAD(_thread, _pfn, _pArg)	_thread = (HANDLE)_beginthread(_pfn, 0, _pArg)    // 返回值0表示错误
#define END_THREAD()                        _endthread();
#define FAILED_THREAD(_t)                   (_t) <= 0
#define SUCCEED_THREAD(_t)                  (_t) > 0
#define usleep(_us)						    Sleep(_us / 1000)  

// Socket 通信
#define INIT_SOCKET					
#define CLOSE_SOCKET(_socket)				closesocket(_socket)
#define socklen_t							int

// 目录操作
#include <sys/stat.h>
#include <direct.h>
#include <io.h>
#define S_ISDIR(m)							(m & _S_IFDIR)
#define FIND_DATA_T							_finddata_t
#define OPEN_DIR(_dir, _path)				long _dir;
#define INVALID_DIR(_dir)					0
#define FIND_FIRST(_dir, _path, _data)      _dir = (long)_findfirst(_path, &_data)
#define FIND_NEXT(_dir, _data)				_findnext(_dir, &_data)
#define VALID_FIND(_f)						(_f) != -1
#define INVALID_FIND(_f)					(_f) == -1
#define GET_FILENAME(_data)					_data.name
#define GET_FILEATTRIB(_data)				_data.attrib
#define FIND_CLOSE(_dir)					_findclose(_dir)
#define DIR_EXIST(_path)					(_access(_path, 0) != -1)
#define MAKE_DIR(_dir)						_mkdir(_dir)
#define UNLINK(_f)							_unlink(_f)

// 动态链接库加载
#define DEFINE_LIBRARY(_dll)				HINSTANCE _dll
#define LOAD_LIBRARY(_dllfile)				LoadLibrary(_dllfile)
#define GET_FUNC(_dll, _func)				GetProcAddress(_dll, _func)
#define FREE_LIBRARY(_dll)					FreeLibrary(_dll)
#define SystemGetLastError()				WSAGetLastError()
#define SYSTEM_ERROR_EWOULDBLOCK			WSAEWOULDBLOCK 
#define SYSTEM_ERROR_EINVAL					WSAEINVAL
#define SYSTEM_ERROR_NOTINITIALISED			WSANOTINITIALISED
#define SYSTEM_ERROR_ENETDOWN				WSAENETDOWN
#define SYSTEM_ERROR_ENOTSOCK				WSAENOTSOCK
#define SYSTEM_ERROR_EFAULT					WSAEFAULT

// 其它
#define RTLINE  "\r\n" 

#else   // Linux平台

#include <pthread.h>
#include <stdint.h>
#include <dirent.h>

#define _off_t								off_t
//#define __int64                           long long
typedef int64_t								__int64;

// 锁资源宏定义
#define DEFINE_LOCKER(_locker)				pthread_mutex_t _locker
#define INITIALIZE_LOCKER(_locker)			pthread_mutex_init(&_locker, NULL)
#define INVALID_LOCKER(_locker)				0
#define LOCK(_locker)						pthread_mutex_lock(&_locker)
#define UNLOCK(_locker)						pthread_mutex_unlock(&_locker)
#define DESTROY_LOCKER(_locker)				pthread_mutex_destroy(&_locker)

// 事件的宏定义
#define DEFINE_EVENT(_event)				pthread_cond_t _event
#define INITIALIZE_EVENT(_event)			_event = PTHREAD_COND_INITIALIZER
#define INVALID_EVENT(_event)				0
#define BROADCAST_EVENT(_event)				pthread_cond_broadcast(&_event)
#define WAIT_EVENT(_event, _locker)			pthread_cond_wait(&_event, &_locker)
#define DESTROY_EVENT(_event)				pthread_cond_destroy(&_event)

// 线程相关的宏定义
#define DEFINE_THREAD(_thread)				pthread_t _thread
#define BEGIN_THREAD(_thread, _pfn, _pArg)	pthread_create(&_thread, NULL, _pfn, _pArg)
#define END_THREAD()                        ;
#define FAILED_THREAD(_t)                   (_t) != 0
#define SUCCEED_THREAD(_t)                  (_t) == 0

// Socket通信
#include <sys/select.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/errno.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define SOCKET								int
#define INVALID_SOCKET						-1
#define SOCKET_ERROR						-1
#define CLOSE_SOCKET(_socket)				close(_socket)
#define ioctlsocket(_a, _b, _c)				ioctl(_a, _b, _c)
#define SystemGetLastError()				errno
#define SYSTEM_ERROR_EWOULDBLOCK			EINPROGRESS
#define SYSTEM_ERROR_EINVAL					EINVAL
#define SYSTEM_ERROR_NOTINITIALISED			10093
#define SYSTEM_ERROR_ENETDOWN				ENETDOWN
#define SYSTEM_ERROR_ENOTSOCK				ENOTSOCK
#define SYSTEM_ERROR_EFAULT					EFAULT

// 目录操作
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <dirent.h>
#ifdef S_ISDIR
	#undef S_ISDIR
	#define S_ISDIR(m)						(m & 0x4)
#endif
#define FIND_DATA_T							struct dirent*
#define OPEN_DIR(_dir, _path)				DIR* _dir = opendir(_path)
#define INVALID_DIR(_dir)					_dir == NULL
#define FIND_FIRST(_dir, _path, _data)      _data = readdir(_dir)
#define FIND_NEXT(_dir, _data)				( (_data = readdir(_dir)) != NULL ? 1: -1)
#define VALID_FIND(_f)						(_f) != NULL
#define INVALID_FIND(_f)					(_f) == NULL
#define GET_FILENAME(_data)					_data->d_name
#define GET_FILEATTRIB(_data)				_data->d_type
#define FIND_CLOSE(_dir)					closedir(_dir)
#define DIR_EXIST(_path)                    (access(_path, 0) != -1)
#define MAKE_DIR(_dir)						mkdir(_dir,0755)
#define UNLINK(_f)							unlink(_f)

// 动态链接库加载
#include <dlfcn.h>
#define DEFINE_LIBRARY(_dll)				void* _dll
#define LOAD_LIBRARY(_dllfile)				dlopen(_dllfile, RTLD_LAZY)
#define GET_FUNC(_dll, _func)				dlsym(_dll, _func)
#define FREE_LIBRARY(_dll)					dlclose(_dll)

//Function
#define _strnicmp							strncasecmp
#define _stricmp							strcasecmp
#define _snprintf                           snprintf

// 其它
#define RTLINE  "\n" 

#endif   // #ifdef _WIN32

#endif //H_SYSTEMMACRODEF_H
