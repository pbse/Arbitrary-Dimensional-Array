/*
 * test.cpp
 *
 *  Created on: May 6, 2015
 *  Author: Prashant Bansal
 *  Original Assignment By: Dr. Kenneth Chiu
 */

#include "Array.hpp"
#include <iostream>
#include <stdio.h>
#include <assert.h>

void initialtest(){
    ndarray::Array<int, 3, 3, 3> a, b;
    int counter = 0;
    for(int i=0; i<3; i++){
        for(int j=0; j<3; j++){
            for(int k=0; k<3; k++){
                a[i][j][k] = counter;
                b[k][j][i] = counter;
                counter++;
            }
        }
    }
    std::cout<<"Total Elements Added - " << counter <<std::endl;
    counter=0;
    for(auto it = a.fmbegin(); it != a.fmend(); ++it){
        assert(*it == counter);
        counter++;
    }
    std::cout<<"Total Elements Iterated in First Dimension Order - " << counter <<std::endl;
    counter=0;
    for(auto it = b.lmbegin(); it != b.lmend(); ++it){
        assert(*it == counter);
        counter++;
    }

    std::cout<<"Total Elements Iterated in Last Dimension Order - " << counter<<std::endl;

}

void maintest() {
		ndarray::Array<int, 2, 3, 4> a, b;
		ndarray::Array<short, 2, 3, 4> c;
		// Initialize.
		int counter = 0;
		for (int i = 0; i < 2; i++) {
		  for (int j = 0; j < 3; j++) {
		     for (int k = 0; k < 4; k++) {
		       a[i][j][k] = b[i][j][k] = counter;
		       c[i][j][k] = counter;
		       counter++;
		     }
		  }
		}
		c[0][0][0] = 1;
		std::cout << c[1][0][3] << std::endl;

		// Range-checked indexing.
		a[0][0][0] = 1234;
		a[1][1][1] = a[0][0][0];
		a[0][2][3] = 5678; // Set the element in the first plane, 3rd row, and 4th column.
		try {
		  a[0][3][0] = 1; // Out of range, throws.
		  assert(false);
		} catch (ndarray::OutOfRange) {
		}

		std::cout << a[0][0][0] << std::endl;
		const ndarray::Array<int, 2, 3, 4> &const_ref(a);
		int i = const_ref[0][0][0]; // Okay.
		a = a; // Self-assignment must be a no-op.
		b = a; // Okay, as long as dimensions and type match exactly.
		a = c; // Member template constructor.
		std::cout << a[0][0][0] << std::endl;
		std::cout << c[0][0][0] << std::endl;

		// Iterate through in plane major order.
		std::cout << "First dimension major (row-major in 2-D): " << std::endl;
		for (ndarray::Array<int, 2, 3, 4>::FirstDimensionMajorIterator it = a.fmbegin(); it != a.fmend(); ++it) {
		  std::cout << *it << std::endl;
		}
		std::cout << "Printing Complete\n";
		int k=0;
		// Iterate through in column major order.
		std::cout << "Last dimension major (column-major in 2-D): " << std::endl;
		for (ndarray::Array<int, 2, 3, 4>::LastDimensionMajorIterator it = a.lmbegin(); it != a.lmend(); ++it) {
		  std::cout << *it << std::endl;
		  k++;
		}
		std::cout << "Printing Complete - i " << k << "\n";

}

int main() {
	initialtest();
	maintest();
	return 0;
}
