// ******************************* //
//            @COPYLEFT            //
//       ALL WRONGS RESERVED       //
// ******************************* //

#pragma once

namespace uy
{
    namespace detail
    {
        template <typename T>
        struct dereference
        {
            typedef T& type;
        };

        template <>
        struct dereference<void>
        {
            typedef void type;
        };

        class ref_counted_base
        {
        private:
            long value;

        public:
            ref_counted_base(long init_val) : value(init_val) {}

            virtual ~ref_counted_base() {}

            void add_ref()
            {
                ++value;
            }

            void release()
            {
                if (--value == 0)
                {
                    this->dispose();
                    delete this;
                }
            }

            virtual void dispose() = 0;
        };

        template <typename T>
        class ref_counted : public ref_counted_base
        {
        private:
            T* target;

        public:
            ref_counted(T* target, long init_val) : ref_counted_base(init_val), target(target) {}

            virtual void dispose()
            {
                delete target;
            }
        };

        class ctrl_block
        {
        private:
            ref_counted_base* ref_counted;

        public:
            ctrl_block() throw() : ref_counted() {}

            ctrl_block(ref_counted_base* ref_counted) : ref_counted(ref_counted) {}

            ctrl_block(const ctrl_block& that) throw() : ref_counted(that.ref_counted)
            {
                if (this->ref_counted)
                {
                    this->ref_counted->add_ref();
                }
            }

            ~ctrl_block()
            {
                if (this->ref_counted)
                {
                    this->ref_counted->release();
                }
            }

            ctrl_block& operator=(const ctrl_block& that) throw()
            {
                this->ref_counted = that.ref_counted;
                return *this;
            }
        };
    }

    template <typename T>
    class shared_ptr
    {
    private:
        T* ptr;
        detail::ctrl_block ctrl;

        template <typename U>
        friend class shared_ptr;

    public:
        shared_ptr() throw() : ptr(), ctrl() {}

        shared_ptr(T* ptr) : ptr(ptr), ctrl(new detail::ref_counted<T>(ptr, 1)) {}

        shared_ptr(const shared_ptr& that) throw() : ptr(that.ptr), ctrl(that.ctrl) {}

        template <typename U>
        shared_ptr(const shared_ptr<U>& that) throw() : ptr(that.ptr), ctrl(that.ctrl) {}

        template <typename U>
        shared_ptr(const shared_ptr<U>& that, T* ptr) throw() : ptr(ptr), ctrl(that.ctrl) {}

        ~shared_ptr() {}

        shared_ptr& operator=(shared_ptr temp) throw()
        {
            T* ptr = this->ptr;
            this->ptr = temp.ptr;
            temp.ptr = ptr;

            detail::ctrl_block ctrl = this->ctrl;
            this->ctrl = temp.ctrl;
            temp.ctrl = ctrl;

            return *this;
        }

        typename detail::dereference<T>::type operator*() const throw()
        {
            return *this->ptr;
        }

        T* get() const throw()
        {
            return this->ptr;
        }

        T* operator->() const throw()
        {
            return this->ptr;
        }

        operator bool() const throw()
        {
            return this->ptr;
        }
    };

    template <typename T, typename U>
    bool operator==(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
    {
        return lhs.get() == rhs.get();
    }

    template <typename T, typename U>
    bool operator!=(const shared_ptr<T>& lhs, const shared_ptr<U>& rhs) throw()
    {
        return lhs.get() != rhs.get();
    }

    template <typename T, typename TSource>
    shared_ptr<T> static_pointer_cast(const shared_ptr<TSource>& that) throw()
    {
        return shared_ptr<T>(that, static_cast<T*>(that.get()));
    }

    template <typename T, typename TSource>
    shared_ptr<T> const_pointer_cast(const shared_ptr<TSource>& that) throw()
    {
        return shared_ptr<T>(that, const_cast<T*>(that.get()));
    }

    template <typename T, typename TSource>
    shared_ptr<T> dynamic_pointer_cast(const shared_ptr<TSource>& that) throw()
    {
        T* p = dynamic_cast<T*>(that.get());
        return p ? shared_ptr<T>(that, p) : shared_ptr<T>();
    }

    template <typename T, typename TSource>
    shared_ptr<T> reinterpret_pointer_cast(const shared_ptr<TSource>& that) throw()
    {
        return shared_ptr<T>(that, reinterpret_cast<T*>(that.get()));
    }

    template <typename T>
    shared_ptr<T> make_shared()
    {
        return shared_ptr<T>(new T);
    }

    template <typename T, typename A1>
    shared_ptr<T> make_shared(const A1& a1)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1)));
    }

    template <typename T, typename A1, typename A2>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2)));
    }

    template <typename T, typename A1, typename A2, typename A3>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3)));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3), const_cast<A4&>(a4)));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3), const_cast<A4&>(a4), const_cast<A5&>(a5)));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3), const_cast<A4&>(a4), const_cast<A5&>(a5), const_cast<A6&>(a6)));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3), const_cast<A4&>(a4), const_cast<A5&>(a5), const_cast<A6&>(a6), const_cast<A7&>(a7)));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3), const_cast<A4&>(a4), const_cast<A5&>(a5), const_cast<A6&>(a6), const_cast<A7&>(a7), const_cast<A8&>(a8)));
    }

    template <typename T, typename A1, typename A2, typename A3, typename A4, typename A5, typename A6, typename A7, typename A8, typename A9>
    shared_ptr<T> make_shared(const A1& a1, const A2& a2, const A3& a3, const A4& a4, const A5& a5, const A6& a6, const A7& a7, const A8& a8, const A9& a9)
    {
        return shared_ptr<T>(new T(const_cast<A1&>(a1), const_cast<A2&>(a2), const_cast<A3&>(a3), const_cast<A4&>(a4), const_cast<A5&>(a5), const_cast<A6&>(a6), const_cast<A7&>(a7), const_cast<A8&>(a8), const_cast<A9&>(a9)));
    }
}
