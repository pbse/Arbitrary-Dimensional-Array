/*
 * Array.hpp
 *
 *  Created on: May 6, 2015
 *  Author: Prashant Bansal
 */

#ifndef ARRAY_HPP_
#define ARRAY_HPP_
#include <stdexcept>

namespace ndarray {

struct OutOfRange : public std::runtime_error {
	OutOfRange(std::string const &msg) : std::runtime_error(msg) {}
};

template <typename T, size_t D, size_t... Dims>
class Array {
public :
	Array<T,Dims...>* arr;
	size_t Dims_Size, initial;
	static T ValueType;
	class FirstDimensionMajorIterator;
	class LastDimensionMajorIterator;

public:
	Array() : arr(nullptr),Dims_Size(sizeof...(Dims)),initial(D) {
		static_assert(D>0,"Wrong Input Initialization");
		arr = new Array<T,Dims...>[initial];
	}
	Array(const Array &ar) : initial(D),Dims_Size(sizeof...(Dims)) {
		if(Dims_Size != ar.Dims_Size || initial !=ar.initial) {
			throw OutOfRange("Wrong Input Const COpy Main");
		}
		arr = new Array<T,Dims...>[initial];
		for(size_t i=0; i < initial; i++){
			arr[i] = ar.arr[i];
		}
	}
	template <typename U>
	Array(const Array<U, D, Dims...>&ar) : initial(D),Dims_Size(sizeof...(Dims)) {
		arr = new Array<T,Dims...>[initial];
		for(size_t i=0; i < initial; i++){
			arr[i] = ar.arr[i];
		}
	}
	Array &operator=(const Array &ar) {
		if(Dims_Size != ar.Dims_Size || initial != ar.initial){
			throw OutOfRange("Wrong Template");
		}
		if(ar.arr != arr){
			for(size_t i=0; i< initial; i++){
				arr[i] = ar.arr[i];
			}
		}
		return *this;
	}
	template <typename U>
	Array &operator=(const Array<U, D, Dims...>&ar) {
		for(size_t i=0; i< initial; i++){
			arr[i] = ar.arr[i];
		}
		return *this;
	}

	Array<T,Dims...> &operator[](const size_t D1) {
		if(D1>=initial) {
			throw OutOfRange("Wrong Input [] Operator");
		}
		return arr[D1];
	}

	const Array<T,Dims...> &operator[](const size_t D1) const{
		if(D1>=initial) {
			throw OutOfRange("Wrong Input Const Operator");
		}
		return arr[D1];
	}

	~Array() {
		delete[] arr;
	}

	// Row Major Order Iterator
	class FirstDimensionMajorIterator {
	public:
		Array<T, D, Dims...> *arr;
		size_t Size,Dims_Cur;
		bool end;
		typename Array<T,Dims...>::FirstDimensionMajorIterator fit;
		FirstDimensionMajorIterator() : arr(nullptr), Size(D), Dims_Cur(0), end(false) {}
		FirstDimensionMajorIterator(const FirstDimensionMajorIterator &it) {
			arr = it.arr;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
			fit = it.fit;
		}
		FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &it) {
			arr = it.arr;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
			fit = it.fit;
			return *this;
		}
		void ChangeDimension() {
			fit.arr = &(arr->arr[Dims_Cur]);
			fit.end = false;
			fit.ChangeDimension();
		}
		FirstDimensionMajorIterator &operator++() {
			++fit;
			if(fit.end && fit.Dims_Cur==0) {
				Dims_Cur += 1;
				if(Dims_Cur == Size) {
					Dims_Cur = 0;
					end = true;
				} else end=false;
				ChangeDimension();
			}
			return *this;
		}
		FirstDimensionMajorIterator operator++(int) {
			FirstDimensionMajorIterator tmp = *this;
			++(*this);
			return tmp;
		}
		T &operator*() const {
			return *fit;
		}
		friend bool operator==(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
			if(it1.arr!=it2.arr || it1.Size!=it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end ) {
				return false;
			}
			if(it1.end) return true;
			return it1.fit == it2.fit;
			}
		friend bool operator!=(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
			if(it1.arr!=it2.arr || it1.Size!=it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end ) {
				return true;
			}
			if(it1.end) return false;
			return it1.fit != it2.fit;
			}
	};

	// Column Major Order Iterator
	class LastDimensionMajorIterator {
	public:
		Array<T, D, Dims...> *arr;
		size_t Size,Dims_Cur;
		bool end;
		typename Array<T,Dims...>::LastDimensionMajorIterator lit;
		LastDimensionMajorIterator() : arr(nullptr), Size(D), Dims_Cur(0), end(false) {}
		LastDimensionMajorIterator(const LastDimensionMajorIterator &it) {
			arr = it.arr;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
			lit = it.lit;
		}
		LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &it) {
			arr = it.arr;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
			lit = it.lit;
			return *this;
		}

		void ChangeDimension() {
			lit.arr = &(arr->arr[Dims_Cur]);
			lit.ChangeDimension();
			end = lit.end;
		}
		LastDimensionMajorIterator &operator++() {
			Dims_Cur = (Dims_Cur + 1)%Size;
			if(Dims_Cur == 0) {
				end = true;
				++lit;
			}
			ChangeDimension();
			return *this;
		}
		LastDimensionMajorIterator operator++(int) {
			LastDimensionMajorIterator tmp = *this;
			++(*this);
			return tmp;
		}
		T &operator*() const {
			return *lit;
		}
		friend bool operator==(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
			if (it1.arr != it2.arr || it1.Size != it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end) {
				return false;
			}
			if (it1.end) return true;
			return it1.lit == it2.lit;
		}
		friend bool operator!=(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
			if (it1.arr != it2.arr || it1.Size != it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end) {
				return true;
			}
			if (it1.end) return false;
			return it1.lit != it2.lit;
		}
	};


//// Iterators Functions  ////
	FirstDimensionMajorIterator fmbegin() {
		FirstDimensionMajorIterator it;
		it.arr = this;
		it.end = false;
		it.fit = arr[0].fmbegin();
		return it;
	}
	FirstDimensionMajorIterator fmend() {
		FirstDimensionMajorIterator it;
		it.arr = this;
		it.end = true;
		it.fit = arr[0].fmend();
		return it;
	}
	LastDimensionMajorIterator lmbegin() {
		LastDimensionMajorIterator it;
		it.arr = this;
		it.end = false;
		it.lit = arr[0].lmbegin();
		return it;
	}
	LastDimensionMajorIterator lmend() {
		LastDimensionMajorIterator it;
		it.arr = this;
		it.end = true;
		it.lit = arr[0].lmend();
		return it;
	}
};


template <typename T, size_t D>
class Array<T,D> {
public:
	static T ValueType;
    T *arr;
    size_t initial;
	class FirstDimensionMajorIterator;
	class LastDimensionMajorIterator;
	Array():arr(nullptr),initial(D) {
		static_assert(D>0,"Wrong Input Initialize");
		arr = new T[initial];
	}

	Array(const Array &ar) : initial(D) {
		if(initial !=ar.initial) {
			throw OutOfRange("Wrong Input Copy");
		}
		arr = new T[initial];
		for(size_t i=0; i < initial; i++){
			arr[i] = ar.arr[i];
		}
	}

	template <typename U>
	Array(const Array<U, D> &ar) : initial(D) {
		arr = new T[initial];
		for(size_t i=0; i < initial; i++){
			arr[i] = ar.arr[i];
		}
	}

	Array& operator=(const Array &ar) {
		if(initial != ar.initial){
			throw OutOfRange("Wrong Template");
		}
		if(arr != ar.arr) {
			for(size_t i=0; i< initial; i++){
				arr[i] = ar.arr[i];
			}
		}
		return *this;
	}

	template <typename U>
	Array& operator=(const Array<U, D> &ar){
		for(size_t i=0; i< initial; i++){
			arr[i] = ar.arr[i];
		}
		return *this;
	}

	T& operator[](const size_t D1) {
		if(D1>=initial) {
			throw OutOfRange("Invalid Range Operator []");
		}
		return arr[D1];
	}

	const T& operator[](const size_t D1) const{
		if(D1>=initial) {
			throw OutOfRange("Invalid Range Operator []");
		}
		return arr[D1];
	}

	~Array() {
		delete[] arr;
	}
	class FirstDimensionMajorIterator {
	public:
		Array<T, D> *arr;
		T *element;
		size_t Size,Dims_Cur;
		bool end;
		FirstDimensionMajorIterator() : arr(nullptr), element(nullptr), Size(D), Dims_Cur(0), end(false) {}
		FirstDimensionMajorIterator(const FirstDimensionMajorIterator &it) {
			arr = it.arr;
			element = it.element;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
		}
		FirstDimensionMajorIterator &operator=(const FirstDimensionMajorIterator &it) {
			arr = it.arr;
			element = it.element;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
			return *this;
		}
		void ChangeDimension() {
			element = &(arr->arr[Dims_Cur]);
		}
		FirstDimensionMajorIterator &operator++() {
			Dims_Cur += 1;
			if(Dims_Cur == Size) {
				Dims_Cur = 0;
				end = true;
			} else end=false;
			ChangeDimension();
			return *this;
		}
		FirstDimensionMajorIterator operator++(int) {
			FirstDimensionMajorIterator tmp = *this;
			++(*this);
			return tmp;
		}
		T &operator*() const {
			return *element;
		}
		friend bool operator==(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
			if(it1.arr!=it2.arr || it1.Size!=it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end ) {
				return false;
			}
			if(it1.end) return true;
			return it1.element == it2.element;
		}
		friend bool operator!=(const FirstDimensionMajorIterator &it1, const FirstDimensionMajorIterator &it2) {
			if(it1.arr!=it2.arr || it1.Size!=it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end ) {
				return true;
			}
			if(it1.end) return false;
			return it1.element != it2.element;
		}
	};
	class LastDimensionMajorIterator {
	public:
		Array<T, D> *arr;
		T *element;
		size_t Size,Dims_Cur;
		bool end;
		LastDimensionMajorIterator() : arr(nullptr), element(nullptr), Size(D), Dims_Cur(0), end(false) {}
		LastDimensionMajorIterator(const LastDimensionMajorIterator &it) {
			arr = it.arr;
			element = it.element;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
		}
		LastDimensionMajorIterator &operator=(const LastDimensionMajorIterator &it) {
			arr = it.arr;
			element = it.element;
			Size = it.Size;
			Dims_Cur = it.Dims_Cur;
			end = it.end;
			return *this;
		}
		void ChangeDimension() {
			element = &(arr->arr[Dims_Cur]);
		}
		LastDimensionMajorIterator &operator++() {
			Dims_Cur += 1;
			if(Dims_Cur == Size) {
				Dims_Cur = Dims_Cur%Size;
				end = true;
			} else end=false;
			ChangeDimension();
			return *this;
		}
		LastDimensionMajorIterator operator++(int) {
			LastDimensionMajorIterator tmp = *this;
			++(*this);
			return tmp;
		}
		T &operator*() const {
			return *element;
		}
		friend bool operator==(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
			if (it1.arr != it2.arr || it1.Size != it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end) {
				return false;
			}
			if (it1.end) return true;
			return it1.element == it2.element;
		}
		friend bool operator!=(const LastDimensionMajorIterator &it1, const LastDimensionMajorIterator &it2) {
			if (it1.arr != it2.arr || it1.Size != it2.Size || it1.Dims_Cur != it2.Dims_Cur || it1.end != it2.end) {
				return true;
			}
			if (it1.end) return false;
			return it1.element != it2.element;
		}
	};

	//Iterator//
	FirstDimensionMajorIterator fmbegin() {
		FirstDimensionMajorIterator it;
		it.arr = this;
		it.element = &arr[0];
		it.end = false;
		return it;
	}
	FirstDimensionMajorIterator fmend() {
		FirstDimensionMajorIterator it;
		it.arr = this;
		it.element = &arr[0];
		it.end = true;
		return it;
	}
	LastDimensionMajorIterator lmbegin() {
		LastDimensionMajorIterator it;
		it.arr = this;
		it.element = &arr[0];
		it.end = false;
		return it;
	}
	LastDimensionMajorIterator lmend() {
		LastDimensionMajorIterator it;
		it.arr = this;
		it.element = &arr[0];
		it.end = true;
		return it;
	}

};
};
#endif /* ARRAY_HPP_ */
