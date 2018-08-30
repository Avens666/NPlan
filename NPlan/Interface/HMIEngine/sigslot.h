//  File:        sigslot.h
//  Copyright:   Copyright(C) 2013-2017 Wuhan KOTEI Informatics Co., Ltd. All rights reserved.
//  Website:     http://www.nuiengine.com
//  Description:  This code is part of NPlan Project (Powered by NUI Engine)
//  Comments:
//  Rev:         1
//  Created:     2018/8/27
//  Last edit:   2016/2/25
//  Author:      Chen Zhi and his team
//  E-mail:       cz_666@qq.com
//  License: APACHE V2.0 (see license file) 

#ifndef SIGSLOT_H__
#define SIGSLOT_H__

#include <set>
#include <list>
#include <boost/thread/thread.hpp>
#include <boost/enable_shared_from_this.hpp>  

#if defined(SIGSLOT_PURE_ISO) || (!defined(WIN32) && !defined(__GNUG__) && !defined(SIGSLOT_USE_POSIX_THREADS))
#	define _SIGSLOT_SINGLE_THREADED
#elif defined(WIN32)
#	define _SIGSLOT_HAS_WIN32_THREADS
#	include <windows.h>
#elif defined(__GNUG__) || defined(SIGSLOT_USE_POSIX_THREADS)
#	define _SIGSLOT_HAS_POSIX_THREADS
#	include <pthread.h>
#else
#	define _SIGSLOT_SINGLE_THREADED
#endif

#ifndef SIGSLOT_DEFAULT_MT_POLICY
#	ifdef _SIGSLOT_SINGLE_THREADED
#		define SIGSLOT_DEFAULT_MT_POLICY single_threaded
#	else
#		define SIGSLOT_DEFAULT_MT_POLICY multi_threaded_local
#	endif
#endif

class KView;
namespace sigslot {

	class single_threaded
	{
	public:
		single_threaded()
		{
			;
		}

		virtual ~single_threaded()
		{
			;
		}

		virtual void lock()
		{
			;
		}

		virtual void unlock()
		{
			;
		}
	};

#ifdef _SIGSLOT_HAS_WIN32_THREADS
	// The multi threading policies only get compiled in if they are enabled.
	class multi_threaded_global
	{
	public:
		multi_threaded_global()
		{
			static bool isinitialised = false;

			if(!isinitialised)
			{
				InitializeCriticalSection(get_critsec());
				isinitialised = true;
			}
		}

		multi_threaded_global(const multi_threaded_global&)
		{
			;
		}

		virtual ~multi_threaded_global()
		{
			;
		}

		virtual void lock()
		{
			EnterCriticalSection(get_critsec());
		}

		virtual void unlock()
		{
			LeaveCriticalSection(get_critsec());
		}

	private:
		CRITICAL_SECTION* get_critsec()
		{
			static CRITICAL_SECTION g_critsec;
			return &g_critsec;
		}
	};

	class multi_threaded_local
	{
	public:
		multi_threaded_local()
		{
			InitializeCriticalSection(&m_critsec);
		}

		multi_threaded_local(const multi_threaded_local&)
		{
			InitializeCriticalSection(&m_critsec);
		}

		virtual ~multi_threaded_local()
		{
			DeleteCriticalSection(&m_critsec);
		}

		virtual void lock()
		{
			EnterCriticalSection(&m_critsec);
		}

		virtual void unlock()
		{
			LeaveCriticalSection(&m_critsec);
		}

	private:
		CRITICAL_SECTION m_critsec;
	};
#endif // _SIGSLOT_HAS_WIN32_THREADS

#ifdef _SIGSLOT_HAS_POSIX_THREADS
	// The multi threading policies only get compiled in if they are enabled.
	class multi_threaded_global
	{
	public:
		multi_threaded_global()
		{
			pthread_mutex_init(get_mutex(), NULL);
		}

		multi_threaded_global(const multi_threaded_global&)
		{
			;
		}

		virtual ~multi_threaded_global()
		{
			;
		}

		virtual void lock()
		{
			pthread_mutex_lock(get_mutex());
		}

		virtual void unlock()
		{
			pthread_mutex_unlock(get_mutex());
		}

	private:
		pthread_mutex_t* get_mutex()
		{
			static pthread_mutex_t g_mutex;
			return &g_mutex;
		}
	};

	class multi_threaded_local
	{
	public:
		multi_threaded_local()
		{
			pthread_mutex_init(&m_mutex, NULL);
		}

		multi_threaded_local(const multi_threaded_local&)
		{
			pthread_mutex_init(&m_mutex, NULL);
		}

		virtual ~multi_threaded_local()
		{
			pthread_mutex_destroy(&m_mutex);
		}

		virtual void lock()
		{
			pthread_mutex_lock(&m_mutex);
		}

		virtual void unlock()
		{
			pthread_mutex_unlock(&m_mutex);
		}

	private:
		pthread_mutex_t m_mutex;
	};
#endif // _SIGSLOT_HAS_POSIX_THREADS

	template<class mt_policy>
	class lock_block
	{
	public:
		mt_policy *m_mutex;

		lock_block(mt_policy *mtx)
			: m_mutex(mtx)
		{
			m_mutex->lock();
		}

		~lock_block()
		{
			m_mutex->unlock();
		}
	};

	template<class mt_policy>
	class has_slots;

	template<class mt_policy>
	class _connection_base0
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit() = 0;
		virtual _connection_base0* clone() = 0;
		virtual _connection_base0* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class mt_policy>
	class _connection_base1
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type) = 0;
		virtual _connection_base1<arg1_type, mt_policy>* clone() = 0;
		virtual _connection_base1<arg1_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class mt_policy>
	class _connection_base2
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type) = 0;
		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* clone() = 0;
		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class arg3_type, class mt_policy>
	class _connection_base3
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type, arg3_type) = 0;
		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* clone() = 0;
		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy>
	class _connection_base4
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type) = 0;
		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* clone() = 0;
		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class mt_policy>
	class _connection_base5
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type) = 0;
		virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy>* clone() = 0;
		virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class mt_policy>
	class _connection_base6
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
			arg6_type) = 0;
		virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy>* clone() = 0;
		virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class mt_policy>
	class _connection_base7
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
			arg6_type, arg7_type) = 0;
		virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy>* clone() = 0;
		virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy>
	class _connection_base8
	{
	public:
		virtual has_slots<mt_policy>* getdest() const = 0;
		virtual void emit(arg1_type, arg2_type, arg3_type, arg4_type, arg5_type,
			arg6_type, arg7_type, arg8_type) = 0;
		virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>* clone() = 0;
		virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest) = 0;
	};

	template<class mt_policy>
	class _signal_base : public mt_policy
	{
	public:
		virtual void slot_disconnect(has_slots<mt_policy>* pslot) = 0;
		virtual void slot_duplicate(const has_slots<mt_policy>* poldslot, has_slots<mt_policy>* pnewslot) = 0;
	};

	template<class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class has_slots : public mt_policy 
	{
	private:
		typedef std::set<_signal_base<mt_policy> *> sender_set;
		typedef typename sender_set::const_iterator const_iterator;

	public:
		has_slots()
		{
			;
		}

		has_slots(const has_slots& hs)
			: mt_policy(hs)
		{
			//lock_block<mt_policy> lock(this);
			//const_iterator it = hs.m_senders.begin();
			//const_iterator itEnd = hs.m_senders.end();

			//while(it != itEnd)
			//{
			//	(*it)->slot_duplicate(&hs, this);
			//	m_senders.insert(*it);
			//	++it;
			//}
		} 

		void signal_connect(_signal_base<mt_policy>* sender)
		{
			//lock_block<mt_policy> lock(this);
			//m_senders.insert(sender);
		}

		void signal_disconnect(_signal_base<mt_policy>* sender)
		{
			//lock_block<mt_policy> lock(this);
			//m_senders.erase(sender);
		}

		virtual ~has_slots()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			//lock_block<mt_policy> lock(this);
			//const_iterator it = m_senders.begin();
			//const_iterator itEnd = m_senders.end();

			//while(it != itEnd)
			//{
			//	(*it)->slot_disconnect(this);
			//	++it;
			//}

			//m_senders.erase(m_senders.begin(), m_senders.end());
		}

	private:
//		sender_set m_senders;
	};

	template<class mt_policy>
	class _signal_base0 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base0<mt_policy> *>  connections_list;


		_signal_base0()
		{
			;
		}

		_signal_base0(const _signal_base0& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		~_signal_base0()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

	protected:
		connections_list m_connected_slots;
	};

	template<class arg1_type, class mt_policy>
	class _signal_base1 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base1<arg1_type, mt_policy> *>  connections_list;

		_signal_base1()
		{
			;
		}

		_signal_base1(const _signal_base1<arg1_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base1()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}


	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class mt_policy>
	class _signal_base2 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base2<arg1_type, arg2_type, mt_policy> *>
			connections_list;

		_signal_base2()
		{
			;
		}

		_signal_base2(const _signal_base2<arg1_type, arg2_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base2()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class arg3_type, class mt_policy>
	class _signal_base3 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base3<arg1_type, arg2_type, arg3_type, mt_policy> *>
			connections_list;

		_signal_base3()
		{
			;
		}

		_signal_base3(const _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base3()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy>
	class _signal_base4 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base4<arg1_type, arg2_type, arg3_type,
			arg4_type, mt_policy> *>  connections_list;

		_signal_base4()
		{
			;
		}

		_signal_base4(const _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base4()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class mt_policy>
	class _signal_base5 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base5<arg1_type, arg2_type, arg3_type,
			arg4_type, arg5_type, mt_policy> *>  connections_list;

		_signal_base5()
		{
			;
		}

		_signal_base5(const _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base5()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class mt_policy>
	class _signal_base6 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base6<arg1_type, arg2_type, arg3_type, 
			arg4_type, arg5_type, arg6_type, mt_policy> *>  connections_list;

		_signal_base6()
		{
			;
		}

		_signal_base6(const _signal_base6<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base6()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class mt_policy>
	class _signal_base7 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base7<arg1_type, arg2_type, arg3_type, 
			arg4_type, arg5_type, arg6_type, arg7_type, mt_policy> *>  connections_list;

		_signal_base7()
		{
			;
		}

		_signal_base7(const _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base7()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy>
	class _signal_base8 : public _signal_base<mt_policy>
	{
	public:
		typedef std::list<_connection_base8<arg1_type, arg2_type, arg3_type, 
			arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy> *>
			connections_list;

		_signal_base8()
		{
			;
		}

		_signal_base8(const _signal_base8<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>& s)
			: _signal_base<mt_policy>(s)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = s.m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = s.m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_connect(this);
				m_connected_slots.push_back((*it)->clone());

				++it;
			}
		}

		void slot_duplicate(const has_slots<mt_policy>* oldtarget, has_slots<mt_policy>* newtarget)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == oldtarget)
				{
					m_connected_slots.push_back((*it)->duplicate(newtarget));
				}

				++it;
			}
		}

		~_signal_base8()
		{
			disconnect_all();
		}

		void disconnect_all()
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::const_iterator it = m_connected_slots.begin();
			typename connections_list::const_iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				//(*it)->getdest()->signal_disconnect(this);
				delete *it;

				++it;
			}

			m_connected_slots.erase(m_connected_slots.begin(), m_connected_slots.end());
		}

		void disconnect(has_slots<mt_policy>* pclass)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				if((*it)->getdest() == pclass)
				{
					delete *it;
					m_connected_slots.erase(it);
					//pclass->signal_disconnect(this);
					return;
				}

				++it;
			}
		}

		void slot_disconnect(has_slots<mt_policy>* pslot)
		{
			lock_block<mt_policy> lock(this);
			typename connections_list::iterator it = m_connected_slots.begin();
			typename connections_list::iterator itEnd = m_connected_slots.end();

			while(it != itEnd)
			{
				typename connections_list::iterator itNext = it;
				++itNext;

				if((*it)->getdest() == pslot)
				{
					m_connected_slots.erase(it);
					//			delete *it;
				}

				it = itNext;
			}
		}

	protected:
		connections_list m_connected_slots;   
	};


	template<class dest_type, class mt_policy>
	class _connection0 : public _connection_base0<mt_policy>
	{
	public:
		_connection0()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection0(dest_type* pobject, void (dest_type::*pmemfun)())
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
				
			}
			m_pmemfun = pmemfun;
		}

		virtual _connection_base0<mt_policy>* clone()
		{
			return new _connection0<dest_type, mt_policy>(*this);
		}

		virtual _connection_base0<mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection0<dest_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit()
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)();
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; //zhic 断掉信号和槽的双向联系 使用智能指针虚指针机制保护
		void (dest_type::* m_pmemfun)();
	};

	template<class dest_type, class arg1_type, class mt_policy>
	class _connection1 : public _connection_base1<arg1_type, mt_policy>
	{
	public:
		_connection1()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection1(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				//m_weak_p = ((boost::enable_shared_from_this<dest_type>*)pobject)->shared_from_this();
				m_weak_p = pobject->shared_from_this();
			}
			m_pmemfun = pmemfun;
		}

		virtual _connection_base1<arg1_type, mt_policy>* clone()
		{
			return new _connection1<dest_type, arg1_type, mt_policy>(*this);
		}

		virtual _connection_base1<arg1_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection1<dest_type, arg1_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; //zhic 断掉信号和槽的双向联系 使用智能指针虚指针机制保护
		void (dest_type::* m_pmemfun)(arg1_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class mt_policy>
	class _connection2 : public _connection_base2<arg1_type, arg2_type, mt_policy>
	{
	public:
		_connection2()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection2(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}
			m_pmemfun = pmemfun;
		}

		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* clone()
		{
			return new _connection2<dest_type, arg1_type, arg2_type, mt_policy>(*this);
		}

		virtual _connection_base2<arg1_type, arg2_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection2<dest_type, arg1_type, arg2_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1, a2);
			}
			
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; //zhic 断掉信号和槽的双向联系 使用智能指针虚指针机制保护
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type, class mt_policy>
	class _connection3 : public _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>
	{
	public:
		_connection3()
		{
			m_pobject = NULL;
			m_pobject = NULL;
		}

		_connection3(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type, arg3_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}
			m_pmemfun = pmemfun;
		}

		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* clone()
		{
			return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, mt_policy>(*this);
		}

		virtual _connection_base3<arg1_type, arg2_type, arg3_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection3<dest_type, arg1_type, arg2_type, arg3_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3)
		{
			
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1, a2, a3);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; 
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class mt_policy>
	class _connection4 : public _connection_base4<arg1_type, arg2_type,
		arg3_type, arg4_type, mt_policy>
	{
	public:
		_connection4()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection4(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}
			m_pmemfun = pmemfun;
		}

		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* clone()
		{
			return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>(*this);
		}

		virtual _connection_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection4<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, 
			arg4_type a4)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1, a2, a3, a4);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; 
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type,
			arg4_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class mt_policy>
	class _connection5 : public _connection_base5<arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, mt_policy>
	{
	public:
		_connection5()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection5(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}

			m_pmemfun = pmemfun;
		}

		virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, mt_policy>* clone()
		{
			return new _connection5<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, mt_policy>(*this);
		}

		virtual _connection_base5<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection5<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; 
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class arg6_type, class mt_policy>
	class _connection6 : public _connection_base6<arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, arg6_type, mt_policy>
	{
	public:
		_connection6()
		{
			m_pobject = NULL;
			m_pobject = NULL;
		}

		_connection6(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}

			m_pmemfun = pmemfun;
		}

		virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, arg6_type, mt_policy>* clone()
		{
			return new _connection6<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, arg6_type, mt_policy>(*this);
		}

		virtual _connection_base6<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, arg6_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection6<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, arg6_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; 
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class arg6_type, class arg7_type, class mt_policy>
	class _connection7 : public _connection_base7<arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>
	{
	public:
		_connection7()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection7(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}

			m_pmemfun = pmemfun;
		}

		virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, arg6_type, arg7_type, mt_policy>* clone()
		{
			return new _connection7<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, arg6_type, arg7_type, mt_policy>(*this);
		}

		virtual _connection_base7<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, arg6_type, arg7_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection7<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, arg6_type, arg7_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6, arg7_type a7)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
			(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6, a7);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; 
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type);
	};

	template<class dest_type, class arg1_type, class arg2_type, class arg3_type,
	class arg4_type, class arg5_type, class arg6_type, class arg7_type, 
	class arg8_type, class mt_policy>
	class _connection8 : public _connection_base8<arg1_type, arg2_type,
		arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>
	{
	public:
		_connection8()
		{
			m_pobject = NULL;
			m_pmemfun = NULL;
		}

		_connection8(dest_type* pobject, void (dest_type::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, 
			arg7_type, arg8_type))
		{
			m_pobject = pobject;
			if (pobject)
			{
				m_weak_p = pobject->shared_from_this();
			}

			m_pmemfun = pmemfun;
		}

		virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>* clone()
		{
			return new _connection8<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>(*this);
		}

		virtual _connection_base8<arg1_type, arg2_type, arg3_type, arg4_type, 
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>* duplicate(has_slots<mt_policy>* pnewdest)
		{
			return new _connection8<dest_type, arg1_type, arg2_type, arg3_type, arg4_type, 
				arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>((dest_type *)pnewdest, m_pmemfun);
		}

		virtual void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
		{
			if ( !m_weak_p._empty() &&  !m_weak_p.expired())
			{
				(m_pobject->*m_pmemfun)(a1, a2, a3, a4, a5, a6, a7, a8);
			}
		}

		virtual has_slots<mt_policy>* getdest() const
		{
			return m_pobject;
		}

	private:
		dest_type* m_pobject;
		boost::weak_ptr<KView> m_weak_p; 
		void (dest_type::* m_pmemfun)(arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type);
	};

	template<class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal0 : public _signal_base0<mt_policy>
	{
	public:
		signal0()
		{
			;
		}

		signal0(const signal0<mt_policy>& s)
			: _signal_base0<mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)())
		{
			lock_block<mt_policy> lock(this);
			_connection0<desttype, mt_policy>* conn = 
				new _connection0<desttype, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit()
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base0<mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base0<mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit();

				it = itNext;
			}
		}

		void operator()()
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base0<mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base0<mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit();

				it = itNext;
			}
		}
	};

	template<class arg1_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal1 : public _signal_base1<arg1_type, mt_policy>
	{
	public:
		signal1()
		{
			;
		}

		signal1(const signal1<arg1_type, mt_policy>& s)
			: _signal_base1<arg1_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type))
		{
			lock_block<mt_policy> lock(this);
			_connection1<desttype, arg1_type, mt_policy>* conn = 
				new _connection1<desttype, arg1_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1);

				it = itNext;
			}
		}

		void operator()(arg1_type a1)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base1<arg1_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1);

				it = itNext;
			}
		}
	};

	template<class arg1_type, class arg2_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal2 : public _signal_base2<arg1_type, arg2_type, mt_policy>
	{
	public:
		signal2()
		{
			;
		}

		signal2(const signal2<arg1_type, arg2_type, mt_policy>& s)
			: _signal_base2<arg1_type, arg2_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type))
		{
			lock_block<mt_policy> lock(this);
			_connection2<desttype, arg1_type, arg2_type, mt_policy>* conn = new
				_connection2<desttype, arg1_type, arg2_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base2<arg1_type, arg2_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base2<arg1_type, arg2_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base2<arg1_type, arg2_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base2<arg1_type, arg2_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2);

				it = itNext;
			}
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal3 : public _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>
	{
	public:
		signal3()
		{
			;
		}

		signal3(const signal3<arg1_type, arg2_type, arg3_type, mt_policy>& s)
			: _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type, arg3_type))
		{
			lock_block<mt_policy> lock(this);
			_connection3<desttype, arg1_type, arg2_type, arg3_type, mt_policy>* conn = 
				new _connection3<desttype, arg1_type, arg2_type, arg3_type, mt_policy>(pclass,
				pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2, arg3_type a3)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2, arg3_type a3)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base3<arg1_type, arg2_type, arg3_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3);

				it = itNext;
			}
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal4 : public _signal_base4<arg1_type, arg2_type, arg3_type,
		arg4_type, mt_policy>
	{
	public:
		signal4()
		{
			;
		}

		signal4(const signal4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>& s)
			: _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type))
		{
			lock_block<mt_policy> lock(this);
			_connection4<desttype, arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>*
				conn = new _connection4<desttype, arg1_type, arg2_type, arg3_type,
				arg4_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base4<arg1_type, arg2_type, arg3_type, arg4_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4);

				it = itNext;
			}
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal5 : public _signal_base5<arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, mt_policy>
	{
	public:
		signal5()
		{
			;
		}

		signal5(const signal5<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy>& s)
			: _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type))
		{
			lock_block<mt_policy> lock(this);
			_connection5<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
				arg5_type, mt_policy>* conn = new _connection5<desttype, arg1_type, arg2_type,
				arg3_type, arg4_type, arg5_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base5<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5);

				it = itNext;
			}
		}
	};


	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal6 : public _signal_base6<arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, mt_policy>
	{
	public:
		signal6()
		{
			;
		}

		signal6(const signal6<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy>& s)
			: _signal_base6<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type, arg6_type))
		{
			lock_block<mt_policy> lock(this);
			_connection6<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
				arg5_type, arg6_type, mt_policy>* conn = 
				new _connection6<desttype, arg1_type, arg2_type, arg3_type,
				arg4_type, arg5_type, arg6_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base6<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base6<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5, a6);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base6<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base6<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5, a6);

				it = itNext;
			}
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal7 : public _signal_base7<arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>
	{
	public:
		signal7()
		{
			;
		}

		signal7(const signal7<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy>& s)
			: _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, 
			arg7_type))
		{
			lock_block<mt_policy> lock(this);
			_connection7<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
				arg5_type, arg6_type, arg7_type, mt_policy>* conn = 
				new _connection7<desttype, arg1_type, arg2_type, arg3_type,
				arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6, arg7_type a7)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5, a6, a7);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6, arg7_type a7)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base7<arg1_type, arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, arg7_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5, a6, a7);

				it = itNext;
			}
		}
	};

	template<class arg1_type, class arg2_type, class arg3_type, class arg4_type,
	class arg5_type, class arg6_type, class arg7_type, class arg8_type, class mt_policy = SIGSLOT_DEFAULT_MT_POLICY>
	class signal8 : public _signal_base8<arg1_type, arg2_type, arg3_type,
		arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>
	{
	public:
		signal8()
		{
			;
		}

		signal8(const signal8<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>& s)
			: _signal_base8<arg1_type, arg2_type, arg3_type, arg4_type,
			arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>(s)
		{
			;
		}

		template<class desttype>
			void connect(desttype* pclass, void (desttype::*pmemfun)(arg1_type,
			arg2_type, arg3_type, arg4_type, arg5_type, arg6_type, 
			arg7_type, arg8_type))
		{
			lock_block<mt_policy> lock(this);
			_connection8<desttype, arg1_type, arg2_type, arg3_type, arg4_type,
				arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>* conn = 
				new _connection8<desttype, arg1_type, arg2_type, arg3_type,
				arg4_type, arg5_type, arg6_type, arg7_type, 
				arg8_type, mt_policy>(pclass, pmemfun);
			this->m_connected_slots.push_back(conn);
			//pclass->signal_connect(this);
		}

		void emit(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base8<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base8<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

				it = itNext;
			}
		}

		void operator()(arg1_type a1, arg2_type a2, arg3_type a3, arg4_type a4,
			arg5_type a5, arg6_type a6, arg7_type a7, arg8_type a8)
		{
			lock_block<mt_policy> lock(this);
			typename _signal_base8<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>::connections_list::const_iterator itNext, it = this->m_connected_slots.begin();
			typename _signal_base8<arg1_type, arg2_type, arg3_type,	arg4_type, arg5_type, arg6_type, arg7_type, arg8_type, mt_policy>::connections_list::const_iterator itEnd = this->m_connected_slots.end();

			while(it != itEnd)
			{
				itNext = it;
				++itNext;

				(*it)->emit(a1, a2, a3, a4, a5, a6, a7, a8);

				it = itNext;
			}
		}
	};

}; // namespace sigslot

#endif // SIGSLOT_H__

