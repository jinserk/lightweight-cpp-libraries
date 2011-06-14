//
// array.h
//
// array class template using partial specialization
//
// Jinserk Baik <jinserk.baik@gmail.com>
// copyright (c) 2011, all rights reserved.
//

#ifndef __ARRAY_H__
#define __ARRAY_H__

#include <iostream>
#include <sstream>

namespace framework
{
    #define SHOW(e)     e.show(__FILE__, __LINE__)

    struct array_exception
    {
        enum category {
            EMPTY,
            NOT_CONTAINER,
            NOT_ELEMENT,
            DIM_ERROR,
            OUT_OF_RANGE,
            NOT_ALLOCATED
        } code_;

        array_exception(category code) : code_(code) {}

        void show(const char* f = __FILE__, const int l = __LINE__)
        {
            std::cerr << "Error in " << f << " at line " << l << " : "
                << what() << std::endl;
        }

        std::string what(void) const
        {
            switch (code_) {
                case EMPTY:
                    return "empty";
                case NOT_CONTAINER:
                    return "the object is not container";
                case NOT_ELEMENT:
                    return "the object is not element";
                case DIM_ERROR:
                    return "dimension error";
                case OUT_OF_RANGE:
                    return "out of range";
                case NOT_ALLOCATED:
                    return "the object is not allocated yet";
                default:
                    return "unknown error";
            }
        }
    }; // struct array_exception

    template <typename T, size_t dim>
    class array
    {
        protected:
            array<T, dim-1>*    container_;
            size_t              sz_;
            size_t              tpos_;

        public:
            array() : container_(NULL), sz_(0), tpos_(0)
            {
                if ((dim < 1) || (dim > 3))
                    throw(array_exception(array_exception::DIM_ERROR));
            }

            array(array<T, dim>& other) : container_(NULL), sz_(0)
            {
                operator= (other);
            }

            array(size_t s1, size_t s2)
            {
                set_size(s1, s2);
            }

            array(size_t s1, size_t s2, size_t s3)
            {
                set_size(s1, s2, s3);
            }

            virtual ~array()
            {
                clear();
            }

            inline void set_size(size_t s1, size_t s2)
            {
                if (dim != 2)
                    throw(array_exception(array_exception::DIM_ERROR));
                container_ = new array<T, dim-1> [s1];
                sz_        = s1;
                tpos_      = 0;
                for (size_t i = 0; i < s1; i++)
                    container_[i].set_size(s2);
            }

            inline void set_size(size_t s1, size_t s2, size_t s3)
            {
                if (dim != 3)
                    throw(array_exception(array_exception::DIM_ERROR));
                container_ = new array<T, dim-1> [s1];
                sz_        = s1;
                tpos_      = 0;
                for (size_t i = 0; i < s1; i++)
                    container_[i].set_size(s2, s3);
            }

            inline virtual void clear()
            {
                if (!container_) return;
                delete [] container_;
                container_ = NULL;
                sz_        = 0;
                tpos_      = 0;
            }

            inline void resize(size_t s1, size_t s2)
            {
                clear();
                set_size(s1, s2);
            }

            inline void resize(size_t s1, size_t s2, size_t s3)
            {
                clear();
                set_size(s1, s2, s3);
            }

            inline virtual array<T, dim-1>& operator[] (size_t idx)
            {
                if (idx >= sz_)
                    throw(array_exception(array_exception::OUT_OF_RANGE));
                if (!container_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                return container_[idx];
            }

            inline virtual array<T, dim-1>& at(size_t idx)
            {
                return operator[](idx);
            }

            inline bool push(const T e)
            {
                if (tpos_ >= sz_) 
                    throw(array_exception(array_exception::OUT_OF_RANGE));
                if (!container_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                if (container_[tpos_].push(e)) ++tpos_;
                if (tpos_ == sz_)
                    return true;
                else
                    return false;
            }

            template <typename T2>
            inline bool push(const T2 e)
            {
                return push((const T)e);
            }

            inline size_t size(const size_t o = 0, const size_t d = dim) const
            {
                if (dim == (d - o))
                    return sz_;
                else
                    return container_[0].size(o, d);
            }

            inline void reset_pos(void)
            {
                tpos_ = 0;
                for (size_t i = 0; i < sz_; i++)
                    container_[i].reset_pos();
            }

            inline array<T, dim>& operator= (array<T, dim>& rhs)
            {
                if (sz_ != rhs.sz_)
                    clear();
                if (!container_) {
                    sz_ = rhs.sz_;
                    container_ = new array<T, dim-1> [sz_];
                }
                tpos_ = rhs.tpos_;
                for (size_t i = 0; i < sz_; i++)
                    container_[i] = rhs.container_[i];
                return *this;
            }

            template <typename T2>
            inline array<T, dim>& operator= (array<T2, dim>& rhs)
            {
                if (sz_ != rhs.size())
                    clear();
                if (!container_) {
                    sz_ = rhs.size();
                    container_ = new array<T, dim-1> [sz_];
                }
                tpos_ = rhs.pos();
                for (size_t i = 0; i < sz_; i++)
                    container_[i] = rhs[i];
                return *this;
            }

            inline array<T, dim>& operator= (const T rhs)
            {
                reset_pos();
                push(rhs);
                return *this;
            }

            template <typename T2>
            inline array<T, dim>& operator= (const T2 rhs)
            {
                reset_pos();
                push((T)rhs);
                return *this;
            }

            inline array<T, dim>& operator+= (const T rhs)
            {
                push(rhs);
                return *this;
            }

    }; // class array<T, dim>

    template<typename T>
    class array<T, 1>
    {
        protected:
            T*      element_;
            size_t  sz_;
            size_t  tpos_;

        public:
            array() : element_(NULL), sz_(0), tpos_(0) {}

            array(array<T, 1>& other) : element_(NULL), sz_(0)
            {
                operator= (other);
            }

            template <typename T2>
            array(array<T2, 1>& other) : element_(NULL), sz_(0)
            {
                operator= (other);
            }

            array(size_t s1)
            {
                set_size(s1);
            }

            virtual ~array()
            {
                clear();
            }

            inline void set_size(size_t s1)
            {
                element_ = new T [s1];
                sz_      = s1;
                tpos_    = 0;
            }

            inline virtual void clear()
            {
                if (!element_) return;
                delete [] element_;
                element_ = NULL;
            }

            inline void resize(size_t s1)
            {
                clear();
                set_size(s1);
            }

            inline virtual T& operator[] (size_t idx)
            {
                if (idx >= sz_)
                    throw(array_exception(array_exception::OUT_OF_RANGE));
                if (!element_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                return element_[idx];
            }

            inline T& at(size_t idx)
            {
                return operator[](idx);
            }

            inline virtual bool push(const T e)
            {
                if (tpos_ >= sz_) 
                    throw(array_exception(array_exception::OUT_OF_RANGE));
                if (!element_)
                    throw(array_exception(array_exception::NOT_ALLOCATED));
                element_[tpos_++] = e;
                if (tpos_ == sz_)
                    return true;
                else
                    return false;
            }

            inline size_t size(const size_t o = 0, const size_t d = 1) const
            {
                return sz_;
            }

            inline array<T, 1>& operator= (array<T, 1>& rhs)
            {
                if (sz_ != rhs.sz_)
                    clear();
                if (!element_) {
                    sz_ = rhs.sz_;
                    element_ = new T [sz_];
                } 
                tpos_ = rhs.tpos_;
                for (size_t i = 0; i < sz_; i++)
                    element_[i] = rhs.element_[i];
                return *this;
            }

            template <typename T2>
            inline array<T, 1>& operator= (array<T2, 1>& rhs)
            {
                if (sz_ != rhs.size())
                    clear();
                if (!element_) {
                    sz_ = rhs.size();
                    element_ = new T [sz_];
                } 
                tpos_ = rhs.pos();
                for (size_t i = 0; i < sz_; i++)
                    element_[i] = (T)rhs[i];
                return *this;
            }

            inline array<T, 1>& operator= (const T rhs)
            {
                reset_pos();
                push(rhs);
                return *this;
            }

            template <typename T2>
            inline array<T, 1>& operator= (const T2 rhs)
            {
                reset_pos();
                push((T)rhs);
                return *this;
            }

            inline array<T, 1>& operator+= (const T rhs)
            {
                push(rhs);
                return *this;
            }

            inline void reset_pos(void) {
                tpos_ = 0;
            }

    }; // class array<T, 1>

    // function templates
    
    template <typename T, size_t dim>
    inline array<T, dim>& operator, (array<T, dim>& ar, const T rhs)
    {
        ar.push(rhs);
        return ar;
    }

    template <typename T1, typename T2, size_t dim>
    inline array<T1,dim>& operator, (array<T1,dim>& ar, const T2 rhs)
    {
        ar.push((T1)rhs);
        return ar;
    }

    template <typename T, size_t dim>
    inline std::ostream& operator<< (std::ostream& os, array<T, dim>& ar)
    {
        std::stringstream oss;
        size_t sz = ar.size();
        if (dim > 1)
            for (size_t i = 0; i < sz; i++) {
                oss.copyfmt(os);
                oss << ar[i] << '\n';
            }
        else
            for (size_t i = 0; i < sz; i++) {
                oss.copyfmt(os);
                oss << ar[i] << ' ';
            }
        return os << oss.str();
    }

} // namespace framework

#endif // __ARRAY_H__

