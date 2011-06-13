//
// buffer.h
//
// circular buffer class 
//
// Jinserk Baik <jinserk.baik@gmail.com>
// copyright (c) 2011, all rights reserved.
//

#ifndef __BUFFER_H__
#define __BUFFER_H__

#include "array.h"

namespace framework
{
    template<typename T>
    class buffer : public array<T, 1>
    {
        using array<T, 1>::element_;
        using array<T, 1>::sz_;
        using array<T, 1>::tpos_;

        private:
            size_t  occupied_;
            size_t  hpos_;

        public:
            buffer() : array<T, 1>(), occupied_(0), hpos_(0) {}

            buffer(size_t s1) : array<T, 1>(s1), occupied_(0), hpos_(0) {}

            buffer(buffer<T>& other)
            {
                element_ = NULL;
                sz_      = 0;
                operator= (other);
            }

            inline virtual void clear()
            {
                array<T, 1>::clear();
                occupied_ = 0;
                hpos_     = 0;
            }

            inline virtual T& operator[] (size_t idx)
            {
                if (idx >= occupied_)
                    throw(array_exception(array_exception::OUT_OF_RANGE));
                if (!element_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                size_t rpos = idx + hpos_;
                if (rpos >= sz_) rpos -= sz_;
                return element_[rpos];
            }

            inline virtual T& at(size_t idx)
            {
                return operator[](idx);
            }

            inline virtual bool push(const T e)
            {
                if (!element_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                element_[tpos_] = e;
                if (++tpos_ == sz_) tpos_ = 0;
                if (++occupied_ > sz_) {
                    if (++hpos_ == sz_) hpos_ = 0;
                    --occupied_;
                }
                return true;
            }

            inline T& pop(void)
            {
                if (!occupied_) 
                    throw(array_exception(array_exception::EMPTY));
                if (!element_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                size_t rpos = hpos_;
                if (++hpos_ == sz_) hpos_ = 0;
                --occupied_;
                return element_[rpos];
            }

            inline size_t occupied(void) const
            {
                return occupied_;
            }

            inline buffer<T>& operator= (buffer<T>& rhs)
            {
                array<T, 1>::operator= ((array<T, 1>&)rhs);
                occupied_ = rhs.occupied_;
                hpos_     = rhs.hpos_;
                return *this;
            }

            template <typename T2>
            inline buffer<T>& operator= (buffer<T2>& rhs)
            {
                array<T, 1>::operator= ((array<T2, 1>&)rhs);
                occupied_ = rhs.occupied_;
                hpos_     = rhs.hpos_;
                return *this;
            }

    }; // class buffer<T>

    template <typename T>
    inline std::ostream& operator<< (std::ostream& os, buffer<T>& bf)
    {
        std::stringstream oss;
        size_t sz = bf.occupied();
        for (size_t i = 0; i < sz; i++) {
            oss.copyfmt(os);
            oss << bf[i] << ' ';
        }
        return os << oss.str();
    }

} // namespace framework

#endif // __BUFFER_H__

