#ifndef _TINY_AUTO_PTR_H
#define _TINY_AUTO_PTR_H

template <class T>
class tiny_auto_ptr {
public:
	explicit tiny_auto_ptr(T* p = 0) : pointee(p) {}

	template <class U>
	tiny_auto_ptr(tiny_auto_ptr<U>& rhs) : pointee(rhs.release()) {}

	~tiny_auto_ptr() {
		delete pointee;
	}

	template <class U>
	tiny_auto_ptr<T>& operator=(tiny_auto_ptr<U>& rhs) noexcept {
		if (this != &rhs)
			reset(rhs.release());
		return *this;
	}

	T& operator*() const noexcept {
		return *pointee;
	}

	T* operator->() const noexcept {
		return pointee;
	}

	T* get() const noexcept {
		return pointee;
	}

	T* release() noexcept {
		T* tmp = pointee;
		pointee = nullptr;
		return tmp;
	}

	void reset(T* p = 0) noexcept{
		if (*p != pointee) {
			delete pointee;
			pointee = p;
		}
	}

private:
	T* pointee;
};

#endif 