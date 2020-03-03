//
// Created by Luoxs on 2016-03-16
// Last modification on 2018-02-02
//
#ifndef __reuse_object_pool_hpp__
#define __reuse_object_pool_hpp__

namespace reuse {

template <typename T>
class object_pool
{
public:
    typedef std::shared_ptr<T> value_type;

    template <typename... Args>
	void init(size_t maxinum, Args&&... args)
	{
		for (size_t i = 0; i < maxinum; ++i)
			pools_.push_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

    value_type instance()
	{
        if (pools_.empty())
            return nullptr;

		T* p = pools_.back().release();
		pools_.pop_back();

        value_type ptr(p, [this](T* t) {
			pools_.emplace_back(t);
		});

		return ptr;
	}

	size_t size()
	{
		return pools_.size();
	}

private:
    std::vector<std::unique_ptr<T>> pools_;
};

} // namespace reuse


#endif // __reuse_object_pool_hpp__
