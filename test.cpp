#include "Rein.h"
#include "HEM.h"
#include<iomanip>
#include <iostream>
//#include <omp.h>
//#include<mpi.h>
//#include <unistd.h>

#include<boost/dynamic_bitset/dynamic_bitset.hpp>
#include <boost/array.hpp>
#include <string>
#include <bitset>
#include<boost/version.hpp>	//包含boost头文件
#include<boost/config.hpp>
#include<boost/thread/thread_pool.hpp>

#include <mmintrin.h>  //MMX
#include <xmmintrin.h> //SSE(include mmintrin.h)
#include <emmintrin.h> //SSE2(include xmmintrin.h)
#include <pmmintrin.h> //SSE3(include emmintrin.h)
#include <tmmintrin.h> //SSSE3(include pmmintrin.h)
#include <smmintrin.h> //SSE4.1(include tmmintrin.h)
#include <nmmintrin.h> //SSE4.2(include smmintrin.h)
#include <wmmintrin.h> //AES(include nmmintrin.h)
#include <immintrin.h> //AVX(include wmmintrin.h)
//#include <intrin.h>    //(include immintrin.h)

int main() {
	int n = 10;
	std::bitset<256> b;
	b[5] = 1;
	b[32] = 1;
	b[33] = 1;
	b[255] = 1;

	//float* a = new float;
	//*a = 11;
	//__m512 m;
	//m = _mm512_load_ps(a);
	//for (int i = 0; i < 16; i++)
	//    cout << m.m512_f32[i] << "\n";

	const int* begin = reinterpret_cast<const int*> (&b);
	int nBlockWidth = 8;
	int* begin2 = new int[16];
	__m256i mask= _mm256_setzero_si256();
	mask[0] = 0x80000000; // .m256i_i32
	mask[1] = 0x80000000; // .m256i_i32
	mask[7] = 0x80000000; // .m256i_i32
	__m256i b256 = _mm256_maskload_epi32(begin, mask);
	//__m512i b512_2 = _mm512_loadu_si512(begin);
	for (int i = 0; i < 8; i++)
		cout << b256[i] << "\n"; //.m256i_i32

	/*boost::dynamic_bitset<unsigned long long> mb(n),b2(n);
	std::cout << "my bitset: " << mb.size() << "位, " << mb << std::endl;
	mb[1] = 1;
	std::cout << "my bitset: " << mb.size() << "位, " << mb << std::endl;
	mb = mb | b2;
	std::cout << "my bitset: " << mb.size() << "位, " << mb << std::endl;*/
	system("PAUSE");
}

//void hello(int& a) {
//	int my_rank = omp_get_thread_num();
//	int thread_count = omp_get_num_threads();
//
//	a = my_rank;
//	//usleep(1000000);
//	printf("hello from rank %d of %d, a= %d\n", my_rank, thread_count, a);
//	fflush(stdout);
//}
//
//void vectorToBitset(vector<int>& v, bitset<subs>*& bt) {
//	cout << "btf1: " << bt << " " << &bt << "\n";
//	for (int i = 0; i < v.size(); i++)
//		(*bt)[v[i]] = 1;
//	v.resize(0);
//}

//bitset<subs>*& vectorToBitset(vector<int>& v) {
//	bitset<subs>* bs = new bitset<subs>;
//	cout << "btf2: " << bs << " " << &bs << "\n";
//	for (auto a : v)
//		(*bs)[a] = 1;
//	v.resize(0);
//	return bs;
//}

//int& t(double d) {
//	int ans = d;
//	cout << "ans= " << &ans << "\n";
//	return ans;
//}

//	int thread_count = 4;
//	int a=5;
//#pragma omp parallel num_threads(thread_count) default(none) shared(a)
//	hello(a);
//bitset<subs>* bs = new bitset<subs>;
//vector<int> v{ 5 };
//cout << "bs1: " << bs << " " << &bs << "\n";
//cout << v.size() << " " << (*bs)[5] << "\n";
//vectorToBitset(v, bs);
//cout << v.size() << " " << (*bs)[5] << "\n";
//v.push_back(999999);
//cout << "bs2: " << bs << " " << &bs << "\n";
//delete bs;
//cout << "bs3: " << bs << " " << &bs << "\n";
//bs = vectorToBitset(v);
//cout << "bs4: " << bs << " " << &bs << "\n";
//cout << v.size() << (*bs)[999999] << " " << (*bs)[5];

//int a = 1;
//cout << "a: " << &a << "\n";
//t(a);
//cout << "a: " << &a << "\n";
//	bitset<1000000> b;
//	b.set();
//	b[999999] = 0;
//	b[12345] = 0;
//	cout << b.count();


//int myid, numprocs, namelen;
//	cout<<MPI_MAX_PROCESSOR_NAME<<"---\n";
//	char processor_name[MPI_MAX_PROCESSOR_NAME];
//	MPI_Init(&argc, &argv);        // starts MPI
//	MPI_Comm_rank(MPI_COMM_WORLD, &myid);  // get current process id
//	MPI_Comm_size(MPI_COMM_WORLD, &numprocs);      // get number of processes
//	MPI_Get_processor_name(processor_name, &namelen);
//	if (myid == 0) printf("number of processes: %d\n...", numprocs);
//	printf("%s: Hello world from process %d \n", processor_name, myid);
//	MPI_Finalize();

