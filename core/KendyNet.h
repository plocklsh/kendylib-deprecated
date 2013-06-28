/*
    Copyright (C) <2012>  <huangweilook@21cn.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _KENDYNET_H
#define _KENDYNET_H
#include <stdint.h>
#include "link_list.h"
//����ϵͳ֧�ֵ�����׽��ֺ�engine������
#define MAX_ENGINE 64
#define MAX_SOCKET 4096

typedef void* ENGINE;
#define INVAILD_ENGINE NULL
typedef void* SOCK;
#define INVAILD_SOCK NULL


#if defined(_LINUX)

/*IO�������ɶ���ʹ�õĽṹ*/
typedef struct
{
	LIST_NODE;
	struct     iovec *iovec;
	int32_t    iovec_count;
	uint32_t   err_code;
}st_io;

#elif defined(_WIN)

#include <winsock2.h>
#include <WinBase.h>

enum
{
	IO_RECV =  1,
	IO_SEND =  2,
};

typedef struct
{
	OVERLAPPED    m_overLapped;
	WSABUF*       iovec;
	DWORD         iovec_count;
	uint8_t       m_Type;
	uint32_t   err_code;
}st_io;

#else
	#error un support os!
#endif

//��ʼ������ϵͳ
int32_t      InitNetSystem();
//recv�������ʱcallback
typedef void (*OnRead)(int32_t,st_io*);
//send�������ʱcallback
typedef void (*OnWrite)(int32_t,st_io*);
//���ӹر�ʱ,������δ��ɵ�����ִ�е�callback
typedef void (*OnClear_pending)(st_io*);

ENGINE   CreateEngine();
void     CloseEngine(ENGINE);
int32_t  EngineRun(ENGINE,int32_t timeout);
int32_t  Bind2Engine(ENGINE,SOCK,OnRead,OnWrite,OnClear_pending);

/*
*  ����ִ��IO����,����ɹ����ؽ��,
*  ���򷵻�-1,err_code == EAGAIN,���׽��ֱ�����ʱ����������������󣬲��ص�ע��ĺ���
*/
int32_t Recv(SOCK,st_io*,uint32_t *err_code);
int32_t Send(SOCK,st_io*,uint32_t *err_code);

/*
* Ͷ�������ڽ�����ĳ��ʱ������������������󣬲��ص�ע��ĺ���
*/
int32_t Post_Recv(SOCK,st_io*);
int32_t Post_Send(SOCK,st_io*);

//int32_t WSASend(HANDLE,st_io*,int32_t flag,uint32_t *err_code);
//int32_t WSARecv(HANDLE,st_io*,int32_t flag,uint32_t *err_code);

#endif