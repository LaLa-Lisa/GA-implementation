#include<iostream>
#include<fstream>
#include<vector>
#include <algorithm>
#include <cmath>
#include <random>
#include"LMatrix.h"
#include"LGenetic.h"

int output_size = 32;
int img_size = 32;
const int layer1_size = img_size * img_size;
const int layer2_size = output_size;
const int layer3_size = output_size;
bool show = false;

std::fstream fin("data.txt");
std::vector<LMatrix> sample(output_size * 2);
std::vector<double> right_ans(output_size * 2);


auto neiron_forward(LMatrix& inVec, LMatrix& weits1, LMatrix& weits2) {
	LMatrix first, second;
	first = inVec * weits1;
	first.sigmoid();
	second = first * weits2;
	//second.sigmoid();
	auto rez = second.getLine();
	double sum = 0;
	for (auto i : rez) sum += exp(i);
	for (auto& i : rez) i = exp(i) / sum;
	return rez;
}

double loss(std::vector<double>& x, std::string file_validation) {
	std::ifstream file(file_validation);
	int n;
	file >> n;
	std::vector<LMatrix> validation(n);
	std::vector<double> true_ans(n);
	for (int i = 0; i < n; ++i) {
		validation[i].init(1, layer1_size);

		for (int j = 0; j < layer1_size; j++)
			file >> validation[i].data[0][j];

		file >> true_ans[i];
	}

	LMatrix weits1(layer1_size, layer2_size);
	LMatrix weits2(layer2_size, layer3_size);
	std::vector<double> line1;
	std::copy(x.begin(), x.begin() + layer1_size * layer2_size, std::back_inserter(line1));
	std::vector<double> line2;
	std::copy(x.begin() + layer1_size * layer2_size, x.end(), std::back_inserter(line2));
	weits1.setLine(line1);
	weits2.setLine(line2);

	double err = 0;
	for (int i = 0; i < n; ++i) {
		auto ans = neiron_forward(validation[i], weits1, weits2);
		int mistake = 0;
		for (int j = 0; j < ans.size(); ++j)
			err += (j == true_ans[i]) ? abs(1 - ans[j]) : abs(ans[j]);
			//err += (j == right_ans[i]) ? abs(1 - ans[j]) : pow(abs(ans[j]), 3);
			//err += (j == true_ans[i]) ? -log(abs(ans[j]) + 0.001) : -log(1.001 - abs(ans[j]));
			//mistake += (j == true_ans[i]) ? abs(1 - (ans[j] >= 0.5)) : (ans[j] >= 0.5);
		//if (mistake != 0) ++err;
	}
	//std::cout << err << "\n";
	return err;
}

double image_fitness(std::vector<double>& x) {
	LMatrix weits1(layer1_size, layer2_size);
	LMatrix weits2(layer2_size, layer3_size);
	std::vector<double> line1;
	std::copy(x.begin(), x.begin() + layer1_size * layer2_size, std::back_inserter(line1));
	std::vector<double> line2;
	std::copy(x.begin() + layer1_size * layer2_size, x.end(), std::back_inserter(line2));
	weits1.setLine(line1);
	weits2.setLine(line2);

	double err = 0;
	for (int i = 0; i < output_size; ++i) {
		auto ans = neiron_forward(sample[i], weits1, weits2);
		for(int j = 0; j < ans.size(); ++j)
			//err += (j == right_ans[i]) ? abs(1 - ans[j]) : abs(ans[j]);
			//err += (j == right_ans[i]) ? pow(abs(1 - ans[j]), 2) : pow(abs(ans[j]), 2);
			err += (j == right_ans[i]) ? -log(abs(ans[j]) + 0.001) : -log(1.001 - abs(ans[j]));
			//err += (j == right_ans[i]) ? exp(-abs(ans[j])) : exp(abs(ans[j]) - 1);
	}
	//err /= 27 * 27;
	
	if (show) {
		int number_of_err = 0;
		for (int i = 0; i < output_size; ++i) {
			auto line = sample[i].getLine();
			std::cout << "Sample:\n";
			for (int j = 0; j < layer1_size; ++j) {
				if (j % img_size == 0) std::cout << '\n';
				std::cout << line[j];
			}
			std::cout << "\n Net answer: \n";
			auto ans = neiron_forward(sample[i], weits1, weits2);
			int mistake = 0;
			for (int j = 0; j < ans.size(); ++j) {
				std::cout << " output " << j << " is " << (ans[j] >= 0.5) << "  [" << ans[j] << "]\n";
				mistake += (j == right_ans[i]) ? abs(1 - (ans[j] >= 0.5)) : (ans[j] >= 0.5);
			}
			if (mistake != 0) std::cout << " GET FUCKED "<< "\n";
			if (mistake != 0) ++number_of_err;
		}
		std::cout << " error " << err << "\n";
		std::cout << " number of errors " << number_of_err << "\n";
	}
	return err;
}

void image_class() {
	int gene_length = layer1_size * layer2_size + layer2_size * layer3_size;
	int pop_size = 32;
	LGenetic Model
	(
		pop_size,
		gene_length,
		image_fitness
	);
	

	std::ifstream gene_read("weits2.txt");
	std::vector<double> weits_from_grad(256 * 27 + 27 * 27);
	for (auto& i : weits_from_grad) gene_read >> i;
	Model.rand_population_normal();
	//Model.read_gene(weits_from_grad);
	//Model.set_crossover(LGenetic::AX);
	//Model.learn(10000);
	//std::vector<double> best = Model.learn_a_lot(500, 10);
	//std::vector<double> best = Model.best_gene();
	//for (int i = 0; i < 10; ++i) std::cout << best[i] << ' '; std::cout << '\n';

	int number_of_tests = 5;
	int iterations = 10000;
	std::vector<std::vector<std::vector<double>>> init_pop_arr(number_of_tests);
	for (auto& i : init_pop_arr)
	{
		i.resize(pop_size);
		srand(time(0));
		std::default_random_engine generator;
		std::uniform_real_distribution<double> distribution(-1, 1);
		//std::normal_distribution<double> distribution{0, 2};
		//std::exponential_distribution<double> distribution(2);
		for (auto& j : i) {
			j.resize(gene_length);
			for (auto& k : j)
				k = distribution(generator);
		}
	}
	
	/*Model.set_crossover(LGenetic::AX);
	Model.make_test("AX.txt", iterations, number_of_tests, init_pop_arr);*/
	/*Model.set_crossover(LGenetic::OX);
Model.make_test("OX.txt", iterations, number_of_tests, init_pop_arr);*/

	Model.file_validation = "valid50.txt";
	Model.set_loss(loss);
	Model.set_mutation(LGenetic::AM);


	Model.set_crossover(LGenetic::SPBX);

	Model.make_test("vsGrad.txt", iterations, number_of_tests, init_pop_arr);


    /*Model.make_test("RWS.txt", iterations, number_of_tests, init_pop_arr);

	Model.set_crossover(LGenetic::SBX);
	Model.make_test("TS.txt", iterations, number_of_tests, init_pop_arr);*/

	/*Model.set_crossover(LGenetic::TPINVX);
	Model.make_test("TPINVX.txt", iterations, number_of_tests, init_pop_arr);

	Model.set_crossover(LGenetic::HX);
	Model.make_test("HX.txt", iterations, number_of_tests, init_pop_arr);*/



	

	std::vector<double> best = Model.best_gene();
	show = true;
	image_fitness(best);
	show = false;
}

void do_it() {
	int gene_length = layer1_size * layer2_size + layer2_size * layer3_size;
	int pop_size = 32;
	LGenetic Model
	(
		pop_size,
		gene_length,
		image_fitness
	);

	Model.rand_population_normal();
	Model.set_crossover(LGenetic::SPBX);
	Model.set_mutation(LGenetic::AM);
	Model.file_validation = "valid50.txt";
	Model.set_loss(loss);
	Model.learn(5000);

	std::vector<double> best = Model.best_gene();
	show = true;
	image_fitness(best);
}

int main() {
	std::fstream sin("train32.txt");
	for (int i = 0; i < output_size * 2; i++)
	{
		sample[i].init(1, layer1_size);

		for (int j = 0; j < layer1_size; j++)
			sin >> sample[i].data[0][j];

		sin >> right_ans[i];

	}
	clock_t start = clock();
	
	//image_class();
	do_it();
	clock_t now = clock();
	std::cout << (double)(now - start) / CLOCKS_PER_SEC << " sec\n";
	return 0;
}