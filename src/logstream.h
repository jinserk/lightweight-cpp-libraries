//
// logstream.h
//

#ifndef __LOGSTREAM_H__
#define __LOGSTREAM_H__

#include <iostream>
#include <fstream>
#include <streambuf>

namespace framework {

    template <class charT, class traits = std::char_traits<charT> >
    class basic_logstreambuf : public std::basic_streambuf<charT, traits>
    {
        public:
            typedef charT                               char_type;
            typedef typename traits::int_type           int_type;
            typedef typename traits::pos_type           pos_type;
            typedef typename traits::off_type           off_type;
            typedef traits                              traits_type;
            typedef std::basic_streambuf<charT, traits> streambuf_type;

        private:
            streambuf_type  *sbuf1_;
            streambuf_type  *sbuf2_;
            bool            sw1_;
            bool            sw2_;
            char_type       *buf_;
            enum {BUFFER_SIZE = 4096 / sizeof(char_type)};

        public:
            basic_logstreambuf(streambuf_type *sbuf1, streambuf_type *sbuf2)
                : sbuf1_(sbuf1), sbuf2_(sbuf2), buf_(new char_type[BUFFER_SIZE])
            {
                sw1_ = sw2_ = true;
                setp(buf_, buf_ + BUFFER_SIZE);
            }

            ~basic_logstreambuf()
            {
                this->pubsync();
                delete[] buf_;
            }

            void sbuf1_on(void)  { sync(); sw1_ = true; }
            void sbuf1_off(void) { sync(); sw1_ = false; }
            void sbuf2_on(void)  { sync(); sw2_ = true; }
            void sbuf2_off(void) { sync(); sw2_ = false; }

        protected:
            virtual int_type overflow(int_type c = traits_type::eof())
            {
                // empty our buffer into sbuf1_ and sbuf2_
                std::streamsize n = static_cast<std::streamsize>(this->pptr() - this->pbase());
                if (sw1_) {
                    std::streamsize size1 = sbuf1_->sputn(this->pbase(), n);
                    if (size1 != n) return traits_type::eof();
                }
                if (sw2_) {
                    std::streamsize size2 = sbuf2_->sputn(this->pbase(), n);
                    if (size2 != n) return traits_type::eof();
                }

                // reset our buffer
                setp(buf_, buf_+BUFFER_SIZE);

                // write the passed character if necessary
                if (!traits_type::eq_int_type(c, traits_type::eof()))
                {
                    traits_type::assign(*this->pptr(), traits_type::to_char_type(c));
                    this->pbump(1);
                }

                return traits_type::not_eof(c);
            }

            virtual int sync()
            {
                // flush our buffer into sbuf1_ and sbuf2_
                int_type c = this->overflow(traits_type::eof());

                // checking return for eof.
                if (traits_type::eq_int_type(c, traits_type::eof()))
                    return -1;

                // flush sbuf1_ and sbuf2_
                if (sw1_)
                    if (sbuf1_->pubsync() == -1) return -1;
                if (sw2_)
                    if (sbuf2_->pubsync() == -1) return -1;

                return 0;
            }

    }; // class basic_logstreambuf<charT, traits>

    typedef basic_logstreambuf<char>        logstreambuf;
    typedef basic_logstreambuf<wchar_t>     wlogstreambuf;

    template <class charT, class traits = std::char_traits<charT> >
    struct scoped_basic_streambuf_assignment
    {
        typedef std::basic_ios<charT, traits>       stream_type;
        typedef std::basic_streambuf<charT, traits> streambuf_type;

        stream_type     &s_;
        streambuf_type  *orig_sb_;

        scoped_basic_streambuf_assignment(stream_type &s, streambuf_type *new_sb) 
            : s_(s)
        {
            orig_sb_ = s_.rdbuf(new_sb);
        }

        ~scoped_basic_streambuf_assignment()
        {
            s_.rdbuf(orig_sb_);
        }

    }; // struct scoped_streambuf_assignment

    typedef scoped_basic_streambuf_assignment<char>    scoped_streambuf_assignment;
    typedef scoped_basic_streambuf_assignment<wchar_t> scoped_wstreambuf_assignment;

    template <class charT, class traits = std::char_traits<charT> >
    class basic_logstream : public std::basic_ostream<charT, traits>
    {
        typedef std::basic_ostream<charT, traits> base_stream_type;

        private:
            std::filebuf  fbuf_;
            logstreambuf  logbuf_;

        public:
            basic_logstream()
                : logbuf_(&fbuf_, std::cout.rdbuf()), base_stream_type(&logbuf_) {}

            basic_logstream(const char* file)
                : logbuf_(&fbuf_, std::cout.rdbuf()), base_stream_type(&logbuf_)
            {
                this->open(file);
            }

            ~basic_logstream()
            {
                this->close();
            }

            virtual void open(const char* file)
            {
                fbuf_.open(file, std::ios::out);
            }

            virtual void close(void)
            {
                base_stream_type::flush();
                if (fbuf_.is_open()) fbuf_.close();
            }

            void fout_on(void)  { logbuf_.sbuf1_on();  }
            void fout_off(void) { logbuf_.sbuf1_off(); }
            void cout_on(void)  { logbuf_.sbuf2_on();  }
            void cout_off(void) { logbuf_.sbuf2_off(); }

    }; // class basic_logstream

    typedef basic_logstream<char>       logstream;
    typedef basic_logstream<wchar_t>    wlogstream;

}; // namespace framework

#endif // __LOGSTREAM_H__

