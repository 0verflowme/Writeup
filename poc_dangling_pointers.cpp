#include<iostream>
#include<memory>

class InnerDb {
public:
	virtual uint64_t getTotalSize() = 0;
};

class InnerDbImpl : InnerDb {
public:
	InnerDbImpl() {};
	uint64_t getTotalSize() {return 100;};
	uint64_t target = 0;
};

class TInstanceImpl {
public:
	TInstanceImpl(InnerDbImpl* inner_db){
		inner_db_ptr_ = inner_db;
	}
	InnerDbImpl* inner_db_ptr_;
};

class TStorageImpl {
public:
	void Init(){
		inner_db_ = std::make_unique<InnerDbImpl>();
	}
	void create(){
		std::make_unique<TInstanceImpl>(inner_db_.get());
	}
	std::unique_ptr<InnerDbImpl> inner_db_;
};


int main(){
	TStorageImpl* ts_ptr = new TStorageImpl();
	ts_ptr->Init();
	InnerDbImpl* tmp_ptr = (ts_ptr->inner_db_).get();
	TInstanceImpl* ti_ptr = new TInstanceImpl(tmp_ptr);

	std::cout << "These two pointers should be same: " << std::endl;
	std::cout << "TStorageImpl->inner_db_: " << (ts_ptr->inner_db_).get() << std::endl;
	std::cout << "TInstanceImpl->inner_db_ptr_: " << ti_ptr->inner_db_ptr_ << std::endl;
	std::cout << "Before: " << std::endl;
	std::cout << "TStorageImpl->inner_db_->target: " << ((ts_ptr->inner_db_).get())->target << std::endl;
	std::cout << "TInstanceImpl->inner_db_ptr_->target: " << (ti_ptr->inner_db_ptr_)->target << std::endl;
	std::cout << "Now TStorageImpl->inner_db_->target change to 100" << std::endl;
	((ts_ptr->inner_db_).get())->target = 100;
	std::cout << "After: " << std::endl;
	std::cout << "TInstanceImpl->inner_db_ptr_->target: " << (ti_ptr->inner_db_ptr_)->target << std::endl;
	std::cout << "Now reset TStorageImpl->inner_db_: " << std::endl;
	ts_ptr->inner_db_.reset();
	std::cout << "TStorageImpl->inner_db_: " << (ts_ptr->inner_db_).get() << std::endl;
	std::cout << "TInstanceImpl->inner_db_ptr_ turns into a dangling pointer" << std::endl;
	std::cout << "TInstanceImpl->inner_db_ptr_: " << ti_ptr->inner_db_ptr_ << std::endl;
	std::cout << "TInstanceImpl->inner_db_ptr_->target: " << (ti_ptr->inner_db_ptr_)->target << std::endl;
}
