#include <stdio.h>
#include <iostream>
#include <boost/thread.hpp>   
#include <boost/date_time.hpp>       
#include <time.h>

static boost::mutex mtx;

const int N = 1000;
float matrixA[N * N];
float matrixB[N * N];
float matrixC1[N * N];
float matrixC2[N * N];

// 各スレッドは、Ａ×Ｂの積Ｃの、idx行の値の計算を実施する
void prodMatrixMT(int idx)
{
	for (int col = 0; col < N; ++col) {
		float sum = 0.0f;
		for (int k = 0; k < N; ++k) {
			sum += matrixA[idx * N + k] * matrixB[k * N + col];
		}

		// idx行、col列の値を更新
		//boost::mutex::scoped_lock lock(mtx);
		{
			matrixC1[idx * N + col] = sum;
		}
	}
}

// 各スレッドは、Ａ×Ｂの積Ｃの、idx行の値の計算を実施する
void prodMatrix(int idx)
{
	for (int col = 0; col < N; ++col) {
		float sum = 0.0f;
		for (int k = 0; k < N; ++k) {
			sum += matrixA[idx * N + k] * matrixB[k * N + col];
		}

		// idx行、col列の値を更新
		matrixC2[idx * N + col] = sum;
	}
}
      
int main(int argc, char* argv[])  
{
	// 行列Ａと行列Ｂを乱数で初期化
	for (int i = 0; i < N * N; ++i) {
		matrixA[i] = (float)(rand() % 100) * 0.01;
		matrixB[i] = (float)(rand() % 100) * 0.01;
	}

	////////////////////////////////////////////////////////////
	// マルチスレッド版、計測開始
	clock_t start = clock();

	// Ａ×Ｂを計算
	boost::thread_group th_group;
	for (int i = 0; i < N; ++i) {
		th_group.create_thread(boost::bind(&prodMatrixMT, i));
	}

	// スレッド実行終了待ち
	th_group.join_all();

	// マルチスレッド版、計測終了
	clock_t end = clock();

	std::cout << "Multi-thread: duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

	////////////////////////////////////////////////////////////
	// シングルスレッド版、計測開始
	start = clock();

	for (int i = 0; i < N; ++i) {
		prodMatrix(i);
	}

	// シングルスレッド版、計測終了
	end = clock();

	std::cout << "Single-thread: duration = " << (double)(end - start) / CLOCKS_PER_SEC << "sec.\n";

	////////////////////////////////////////////////////////////
	// 計算結果の比較
	for (int i = 0; i < N * N; ++i) {
		if (matrixC1[i] != matrixC2[i]) {
			printf("ERROR!!!\n");
		}
	}

    return 0;  
}