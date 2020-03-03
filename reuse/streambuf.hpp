//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_streambuf_hpp__
#define __reuse_streambuf_hpp__


namespace reuse {

class streambuf
{
public:
    struct buffer_type {
        char  *data;
        size_t size;
    };

	explicit streambuf(size_t size = buffer_delta)
        : maxsize_((std::numeric_limits<size_t>::max)())
	{
		size_t alloc_size = (std::min<size_t>)(maxsize_, size);
		buffer_.reset(new char[alloc_size], std::default_delete<char[]>());
		bptr = buffer_.get();
		nptr = bptr;
		eptr = bptr + alloc_size;
	}

	void prepare(size_t n)
	{
		reserve(n);
	}

	void consume(size_t n)
	{
		if (nptr + n <= eptr)
			nptr += n;
	}

	void commit(size_t n)
	{
		if (bptr + n <= nptr)
			bptr += n;
	}

	void reset()
	{
		bptr = buffer_.get();
		nptr = bptr;
	}

    buffer_type idle_buffer()
    {
        return{ nptr, eptr - nptr };
    }

    buffer_type used_buffer()
    {
        return{ bptr, nptr - bptr };
    }

protected:
	enum { buffer_delta = 4096 };

	bool reserve(size_t n)
	{
		size_t consumed_size = bptr - buffer_.get();
		size_t remain_size = eptr - nptr;
		size_t data_size = nptr - bptr;

		if (n <= remain_size)
			return true;

		if (n <= remain_size + consumed_size) {
			memmove(buffer_.get(), bptr, data_size);
			bptr = buffer_.get();
			nptr = bptr + data_size;
		}
		else {
			if (n <= maxsize_ && data_size <= maxsize_ - n) {
				n += data_size;
				decltype(buffer_) newdata(new char[n], std::default_delete<char[]>());
				memmove(newdata.get(), bptr, data_size);
				bptr = newdata.get();
				nptr = bptr + data_size;
				eptr = bptr + n;
				buffer_.swap(newdata);
			}
			else
                return false;
		}
        return true;
	}

	char *bptr;	// beginning of read buffer
	char *eptr; // ending of read buffer
	char *nptr;	// current of read buffer
	size_t maxsize_;
	std::shared_ptr<char> buffer_;
};


} // namespace reuse

#endif // __reuse_streambuf_hpp__
