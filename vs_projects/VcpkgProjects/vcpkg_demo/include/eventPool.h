#ifndef EVENT_POOL_H
#define EVENT_POOL_H

#include <vector>
#include <mutex>
#include <condition_variable>
#include <future>
#include <memory>
#include <queue> // for priority queue.
#include <chrono>
#include <string>
#include <atomic>

/*
*  ��������
*  a header-only event-driven library based on c++11��which use std::promise and std::future asyn-model.
*  һ������c++11��׼������Ҫͷ�ļ����¼�������:)��ʹ��std::promise/std::future�첽ģ��ʵ�֡�
*  ʹ�÷�����
*  1. ����event_pool��������һ���߳����¼������ڸ��߳��е���run������
*  ************************************************
*  //run the event_pool.
   std::shared_ptr<event_pool> ev(new event_pool());
   std::thread th([=]()->void {
	   ev->run();
   });
*  ************************************************
*
*  2. ����event_handle��time_handle��������id_��type_,�ص�����func_��
*  ������args_�������time_handle��Ҫ���ô���ʱ�䣩�ȣ�push��event_pool����
*  *************************************************
*  //create time_handle.
   std::shared_ptr<time_handle> h(new time_handle());
   h->id_ = "timer test ";
   h->type_ = time_handle::type::duration;
   h->duration_ = seconds(2);
   h->args_ = nullptr;
   h->func_ = [](std::shared_ptr<time_handle> self)->void {
		   std::cout << self->id_ << " wake up !" << std::endl;
   };

   //create event_handle.
   std::shared_ptr<event_handle> eh(new event_handle());
   eh->id_ = "back cout ";
   eh->type_ = event_handle::type::every;
   eh->args_ = nullptr;
   eh->func_ = [](std::shared_ptr<event_handle> self)->void {
	   std::cout << self->id_ << " wake up !"<<std::endl;
   };

   //push them into ev.
   ev->push_timer(h);
   ev->push_event(eh);
*  ************************************************
*  3. ����Ҫ�����¼���ʱ�����wake_up������time_handleû��wake_up�������ȴ�ʱ�䵽���Զ���������
*  ����Ҫ�ر�event_poolʱ������stop������Ȼ������̣߳�û�����ü�������¼��ᱻ��������ʹ��event_pool
*  ������ȫ���ٺ���Ȼ���Ե���wake_up��������ʱ��ֱ�ӷ��ء�
*  ************************************************
*     while (true) {
		char buf[1024];
		gets(buf);
		if (buf[0] == 'q') {
		  ev->stop(); // stop the event_pool.
		  break;
		}
		eh->wake_up();
	  }
	  th.join();
   ************************************************
*/

/*
*  ע�����
*  1. ���ж������ʹ��std::shared_ptr������
*  2. ÿ��time_handle�����event_handle����ֻ��push��һ��event_pool����
*  3. event_handle����������������ͣ�every��once��every���������޴�����wake_up,
*  event_pool�ᴦ��ÿ��wake_up����once����ֻ�ܱ�����һ�Σ��������ε���wake_up�������̰߳�ȫ��������ζ�ſ����ڶ���̲߳����Ĵ����¼���
*  4. time_handle����������������ͣ�duration��time_point������duration����ͨ������duration_��Ա��
*  ָ���Ӵ˿̿�ʼ��ÿ�������ʱ��ʹ���һ�Ρ�time_point����ͨ������time_point_��Ա��ָ�����ĸ�ʱ�̽�����һ�Ρ�
*  5. �ص�����������������Ǹ��¼������������ͨ����������õ�id_��type_��args_�ȵȡ�
*  6. event_pool��run���������ڶ���̲߳���ִ�У�maybe��������һ�����Ҳ���֤��
*/

/*
*�ص㣺
* 1.��������200��Դ���룬���Բ���Ŀ�ƽ̨������c++11��׼��
* 2.����Ҫͷ�ļ������ü��á�
*/

/*
* todo:
* ���������ʹ�ã����ٳ�����ʵĽӿڡ�
* ������ԡ�
*/

/* v1.1 update
 * event_pool����set_max_events()�ӿں�������������첽��������,��ǰ�����������ڸ�ֵʱ�������ȴ�����ʼ��Ϊ1024��
 * push_timer��push_event�ӿں������ڷ���std::future<void>�����Ե��øö����get������ʹ�õ�ǰ�߳������ȴ��첽������ɡ�����Щ����²��ɱ����
 * ��Ҫ���첽���������һЩͬ��������
 */

using namespace std::chrono;
class event_pool;

class event_handle : public std::enable_shared_from_this<event_handle> {
	friend class event_pool;
public:
	enum class type { once, every };
	type type_;
	std::string id_;
	std::function<void(std::shared_ptr<event_handle>)> func_;
	std::shared_ptr<void> args_;
	void wake_up();
private:
	std::weak_ptr<event_pool> event_;
	bool trigger_;
	std::atomic<bool> done_; //only for once state event_handle.
	std::shared_ptr<std::promise<void>> promise_;
public:
	event_handle() :trigger_(false), done_(false) {

	}
};

class time_handle : public std::enable_shared_from_this<time_handle> {
	friend class event_pool;
public:
	enum class type { duration, timepoint };
	type type_;
	std::string id_;
	std::function<void(std::shared_ptr<time_handle>)> func_;
	std::shared_ptr<void> args_;
	time_point<system_clock, milliseconds> time_point_; //only for timepoint
	milliseconds duration_; //only fo duration
private:
	std::shared_ptr<std::promise<void>> promise_;
};

class event_pool : public std::enable_shared_from_this<event_pool> {
	friend class event_handle;
	friend class time_handle;
private:
	std::mutex mut_;
	std::condition_variable at_least_one_wake_up_;
	std::condition_variable too_many_events_;
	std::vector<std::shared_ptr<event_handle>> handles_;
	std::vector<std::shared_ptr<event_handle>> active_handles_;
	uint64_t triggers_;
	uint64_t events_;
	uint64_t max_events_;
	bool stop_;

	//for timer.
	using ptr_time_handle = std::shared_ptr<time_handle>;
	struct cmp_for_ptr_handle {
		bool operator()(ptr_time_handle h1, ptr_time_handle h2) {
			return h1->time_point_ > h2->time_point_;
		}
	};

	std::priority_queue<ptr_time_handle, std::vector<ptr_time_handle>, cmp_for_ptr_handle> timers_;

	//hold mutex.
	void remove_all_triggered_handle_and_reset() {
		for (auto it = handles_.begin(); it != handles_.end();) {
			if ((*it)->trigger_ == true) {
				it = handles_.erase(it);
			}
			else {
				++it;
			}
		}
		triggers_ = 0;
	}

	std::vector<std::shared_ptr<event_handle>> get_ready() {
		std::vector<std::shared_ptr<event_handle>> result;
		std::vector<std::shared_ptr<time_handle>> tmp;
		std::unique_lock<std::mutex> mut(mut_);
		if (timers_.empty() == true) {
			at_least_one_wake_up_.wait(mut, [this]()->bool {
				return this->active_handles_.empty() == false || this->timers_.empty() == false || this->stop_ == true;
			});
		}
		else {
			time_point<system_clock, milliseconds> now = time_point_cast<milliseconds>(system_clock::now());
			milliseconds dt = timers_.top()->time_point_ - now;
			at_least_one_wake_up_.wait_for(mut, milliseconds(dt));
		}
		//��ʹ��stop���ѣ�Ҳ�������event��

		//����ɾ���Ѿ���������¼���event_handle����������ڵ���1024����һ����ɾ��һ�Ρ�
		if (triggers_ >= 1024) {
			remove_all_triggered_handle_and_reset();
		}

		time_point<system_clock, milliseconds> now = time_point_cast<milliseconds>(system_clock::now());
		while (timers_.empty() != true) {
			if (timers_.top()->time_point_ <= now) {
				tmp.push_back(timers_.top());
				timers_.pop();
				--events_;
			}
			else {
				break;
			}
		}
		result.swap(active_handles_);
		events_ -= active_handles_.size();
		mut.unlock();
		too_many_events_.notify_all();
		for (auto each : tmp) {
			each->func_(each);
			if (each->promise_) {
				each->promise_->set_value();
				each->promise_.reset();
			}
			if (each->type_ == time_handle::type::duration) {
				push_timer(each);
			}
		}
		return result;
	}

public:
	event_pool() :triggers_(0), events_(0), max_events_(1024), stop_(false) {

	}

	std::future<void> push_event(std::shared_ptr<event_handle> h) {
		std::unique_lock<std::mutex> mut(mut_);
		too_many_events_.wait(mut, [this]()->bool {return this->events_ <= max_events_; });
		h->event_ = shared_from_this();
		h->promise_.reset(new std::promise<void>());
		auto result = h->promise_->get_future();
		handles_.push_back(h);
		++events_;
		return result;
	}

	std::future<void> push_timer(std::shared_ptr<time_handle> h) {
		std::unique_lock<std::mutex> mut(mut_);
		too_many_events_.wait(mut, [this]()->bool {return this->events_ <= max_events_; });
		if (h->type_ == time_handle::type::duration) {
			h->time_point_ = time_point_cast<milliseconds>(system_clock::now()) + h->duration_;
		}
		h->promise_.reset(new std::promise<void>());
		auto result = h->promise_->get_future();
		//no matter which type of h,we always use time_point_ data member.
		timers_.push(h);
		++events_;
		at_least_one_wake_up_.notify_all();
		return result;
	}

	void set_max_events(uint64_t max_events) {
		max_events_ = max_events;
	}

	void stop() {
		std::unique_lock<std::mutex> mut(mut_);
		stop_ = true;
		at_least_one_wake_up_.notify_all();
	}

	void run() {
		while (true) {
			std::vector<std::shared_ptr<event_handle>> ready = get_ready();
			for (std::shared_ptr<event_handle> each : ready) {
				if (each->func_) {
					each->func_(each);
					if (each->promise_) {
						each->promise_->set_value();
						each->promise_.reset();
					}
				}
			}
			std::unique_lock<std::mutex> mut(mut_);
			if (stop_ == true) {
				return;
			}
		}
	}
};

//����once��event��˵��ֻ�ܱ�����һ�Σ���wake_up�����ɶ�ε��ã��̰߳�ȫ��
inline
void event_handle::wake_up() {
	std::shared_ptr<event_pool> hold = event_.lock();
	if (!hold) {
		//invalid event_.
		return;
	}
	bool except_ = false;
	if (type_ == type::once) {
		if (done_.compare_exchange_strong(except_, true) == false) {
			//have been waked_up.
			return;
		}
	}
	std::unique_lock<std::mutex> mut(hold->mut_);
	if (hold->stop_ == true) {
		//event_ stop.
		return;
	}
//	assert(trigger_ == false);
	hold->active_handles_.push_back(shared_from_this());
	if (type_ == type::once) {
		trigger_ = true;
		hold->triggers_ += 1;
	}// esle type_ == type::every, and trigger_ always remain false,so event_ will never delete it.
	hold->at_least_one_wake_up_.notify_all();
}

#endif // EVENT_POOL_H
#pragma once
