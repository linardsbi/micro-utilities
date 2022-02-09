#pragma once

template <typename PtrType>
class unique_ptr {
public:
    unique_ptr(PtrType *ptr)
    : m_ptr(ptr) {}

    unique_ptr(unique_ptr<PtrType> ptr)
    : m_ptr(std::move(ptr)) {}

    T& operator=(const T& other) = delete;

    T& operator=(T&& other) noexcept
	{
	    // Guard self assignment
	    if (this == &other) {
            return *this;
        }

	    delete m_ptr;

	    m_ptr = std::exchange(other.m_ptr, nullptr); // leave other in valid state
	    return *this;
	}

    ~unique_ptr() {
        if (m_ptr != nullptr) {
            delete m_ptr;
        }
    }
private:
    PtrType *m_ptr{nullptr};
};
