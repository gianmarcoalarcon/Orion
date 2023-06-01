#pragma once

#include "parameter.h"
#include <vector>
#include <linear_gkr/prime_field.h>
class graph
{
public:
	int degree;
	std::vector<std::vector<long long>> neighbor;
	std::vector<std::vector<long long>> r_neighbor;
	std::vector<std::vector<prime_field::field_element>> weight;
	std::vector<std::vector<prime_field::field_element>> r_weight;
	long long L, R;
};

extern graph C[100], D[100];

inline graph generate_random_expander(long long L, long long R, long long d)
{
	graph ret;
	ret.degree = d;
	ret.neighbor.resize(L);
	ret.weight.resize(L);

	ret.r_neighbor.resize(R);
	ret.r_weight.resize(R);
	FILE *out1 = fopen("./d_1_neighbor.txt", "w");
	FILE *out2 = fopen("./d_1_weight.txt", "w");
	for (long long i = 0; i < L; ++i)
	{
		ret.neighbor[i].resize(d);
		ret.weight[i].resize(d);
		for (long long j = 0; j < d; ++j)
		{
			long long target = rand() % R;
			prime_field::field_element weight = prime_field::random();
			ret.neighbor[i][j] = target;
			ret.r_neighbor[target].push_back(i);
			ret.r_weight[target].push_back(weight);
			ret.weight[i][j] = weight;
			if (L == 7) // for d_1... L==7
			{
				{
					fprintf(out1, "%lld ", ret.neighbor[i][j]);
					fprintf(out2, "%lld ", ret.weight[i][j]);
				}
			}
		}
		if (L == 7) // for d_1... L==7
		{
			fprintf(out1, "\n");
			fprintf(out2, "\n");
		}
	}
	fclose(out1);
	fclose(out2);

	if (L == 7) // for d_1... L==7
	{
		FILE *out3 = fopen("./d_1_r_neighbor.txt", "w");
		FILE *out4 = fopen("./d_1_r_weight.txt", "w");
		for (int i = 0; i < R; ++i)
		{
			for (int j = 0; j < ret.r_neighbor[i].size(); ++j)
			{
				fprintf(out3, "%lld ", ret.r_neighbor[i][j]);
				fprintf(out4, "%lld ", ret.r_weight[i][j]);
			}
			fprintf(out3, "\n");
			fprintf(out4, "\n");
		}
		fclose(out3);
		fclose(out4);
		exit(0);
	}
	ret.L = L;
	ret.R = R;
	return ret;
}

inline long long expander_init(long long n, int dep = 0)
{
	printf("n:%lld dep:%d\n", n, dep);
	// random graph
	if (n <= distance_threshold)
	{
		return n;
	}
	else
	{
		C[dep] = generate_random_expander(n, (long long)(alpha * n), cn);
		long long L = expander_init((long long)(alpha * n), dep + 1);
		printf("Into D - L:%lld dep: %d\n", L, dep);
		D[dep] = generate_random_expander(L, (long long)(n * (r - 1) - L), dn);
		// exit(0); // stop here to obtein the graph D[0]
		return n + L + (long long)(n * (r - 1) - L);
	}
}