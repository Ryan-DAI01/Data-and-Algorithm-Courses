#include <stdio.h>
#include <math.h>
#include <cstdlib>
#include <ctime>
#include <stdlib.h>
#include <iostream>
#include<fstream>
#pragma warning(disable:4996)

// define global parameters #TODO
double pixel[6001][784];			// 784 = 28 * 28, ??? for length of data
int label[6001];
float learningRate = 0.1;
int m, n, l, maxTrainTimes;
int num_hidden_layer = 2;		// decide how many hidden layers there are
int hidden_nodes[] = {30, 16};	// if num_hidden_layer == 2, hidden_nodes = {num_of_1st_layer_nodes, num_of_2nd_layer_nodes};

/*tools*/
double xavier_uniform(int a, int b) {      // xavier_uniform(in_channel, out_channel)
	double gain = 5;					  // choose a number you like, #TODO
	double x = gain * sqrt(6.0 / (a + b));
	return rand() / (RAND_MAX + 1.0) * 2 * x - x;
}
int getMaxIndex(double* arr) {
	double maxVal = 0.0;
	int maxIdx = 0;
	for (int i = 0; i < l; ++i)
		if (arr[i] > maxVal) {
			maxVal = arr[i];
			maxIdx = i;
		}
	return maxIdx;
}
double* makeOneHot(int label) {
	/* one-hot is an encoding method for classification problem, if labels were "0", "1", "2",
	their one-hot codes are "001", "010", "100" respectively.*/
	double* output = new double[l];
	for (int i = 0; i < l; ++i)
		output[i] = (i == label) ? 1 : 0;
	return output;
}
int findIndex(int label, int* label_tag) {
	for (int i = 0; i < l; ++i)
		if (label == label_tag[i])
			return i;
	return 0;
}

/*define hidden layers*/
class HiddenLayer {
public:
	HiddenLayer(int in_channel, int out_channel);
	~HiddenLayer();

	void forward(double* input_data);
	void backward(double* input_data, double* next_layer_delta, double** next_layer_weight, int next_layer_output);
	void activate_func(double* x);
	void activate_func_bp(double* input_data, double* next_layer_delta,
		double** next_layer_weight, int next_layer_outchannel);

	double* output_data;					// the result passed to the next layer
	double* delta;							// needed for back propagation
	int in_channel;							// input channel
	int out_channel;							// output channel
	double** weight;							// weights of layer
	double* bias;								// bias of layer
};

HiddenLayer::HiddenLayer(int in, int out) {				// initialize network parameters
	in_channel = in;
	out_channel = out;
	weight = new double* [out_channel];
	for (int i = 0; i < out_channel; ++i)
		weight[i] = new double[in_channel];
	bias = new double[out_channel];
	delta = new double[out_channel];
	output_data = new double[out_channel];

	for (int i = 0; i < out_channel; ++i) {					// weights can not be 0, or would not be trained.
		for (int j = 0; j < in_channel; ++j)
			weight[i][j] = xavier_uniform(in_channel, out_channel);
		bias[i] = xavier_uniform(in_channel, out_channel);
	}
}

HiddenLayer::~HiddenLayer() {
	for (int i = 0; i < out_channel; ++i)
		delete[]weight[i];
	delete[] weight;
	delete[] bias;
	delete[] output_data;
	delete[] delta;
}

void HiddenLayer::activate_func(double* x) {
	for (int i = 0; i < out_channel; i++)
		x[i] = 1.0 / (1 + exp(-x[i]));//sigmoid function
}

void HiddenLayer::activate_func_bp(double* input_data, double* next_layer_delta,
	double** next_layer_weight, int next_layer_outchannel) {
	// calculate delta #TODO
	for (int i = 0; i < out_channel; i++)
	{
		delta[i] = 0;
		for (int j = 0; j < next_layer_outchannel; j++)
			delta[i] += next_layer_delta[j] * next_layer_weight[j][i];
	}
	for (int i = 0; i < out_channel; i++)
		delta[i] = output_data[i] * (1 - output_data[i]) * delta[i];//sigmoid:f'=f*(1-f)
}

void HiddenLayer::forward(double* input_data) {
	/*forward data*/
	// calculate between input_data, weight, bias, and get output_data.
	// #TODO
	for (int i = 0; i < out_channel; i++)
	{
		output_data[i] = bias[i];
		for (int j = 0; j < in_channel; j++)
			output_data[i] += weight[i][j] * input_data[j];
	}

	/*activate function*/
	activate_func(output_data);
}
void HiddenLayer::backward(double* input_data, double* next_layer_delta,
	double** next_layer_weight, int next_layer_outchannel) {
	/*
		input_data						the input data of i-th layer
		next_layer_delta				the delta of (i+1)th layer
		next_layer_weight			    the weights propagating from i-th layer to (i+1)th layer
		next_layer_outchannel		    the number of outchannel of (i+1)th layer
		*/

		/*backpropagate of activate function*/
	activate_func_bp(input_data, next_layer_delta,next_layer_weight,next_layer_outchannel);

	/*backpropagation*/
	// calculate new weights and bias #TODO
	for (int i = 0; i < out_channel; i++)
		for (int j = 0; j < in_channel; j++)
			weight[i][j] += learningRate * delta[i] * input_data[j];//partial input/partial weight = before_input

	for (int i = 0; i < out_channel; i++)
		bias[i] += learningRate * delta[i];
}

class OutputLayer {								// almost the same with Hiddenlayer, except using softmax as activate function
public:
	OutputLayer(int in_channel, int out_channel);
	~OutputLayer();

	void forward(double* input_data);
	double backward(double* input_data, double* label);
	void softmax(double* x);
	void softmax_bp(double* input_data, double* label);
	int eval(double* x);

	int in_channel;
	int out_channel;
	double** weight;
	double* bias;
	double* output_data;
	double* delta;
};

OutputLayer::OutputLayer(int in, int out) {		// initialize network parameters
	in_channel = in;
	out_channel = out;
	weight = new double* [out_channel];
	for (int i = 0; i < out_channel; ++i)
		weight[i] = new double[in_channel];
	bias = new double[out_channel];
	delta = new double[out_channel];
	output_data = new double[out_channel];

	for (int i = 0; i < out_channel; ++i) {
		for (int j = 0; j < in_channel; ++j)
			weight[i][j] = xavier_uniform(in_channel, out_channel);
		bias[i] = xavier_uniform(in_channel, out_channel);
	}
}

OutputLayer::~OutputLayer() {
	for (int i = 0; i < out_channel; ++i)
		delete[]weight[i];
	delete[] weight;
	delete[] bias;
	delete[] output_data;
	delete[] delta;
}

void OutputLayer::softmax(double* x) {
	double max = 0.0, sum = 0.0;
	for (int i = 0; i < out_channel; ++i)
		if (max < x[i])
			max = x[i];
	for (int i = 0; i < out_channel; ++i) {
		x[i] = exp(x[i] - max);
		sum += x[i];
	}
	for (int i = 0; i < out_channel; ++i)
		x[i] /= sum;
}

void OutputLayer::softmax_bp(double* input_data, double* label) {
	// calculate delta #TODO
	for (int i = 0; i < out_channel; i++)
		delta[i] = label[i] - output_data[i];//softmax:partial f/ partial input = output - label
}

void OutputLayer::forward(double* input_data) {
	/*forward data*/
	// calculate between input_data, weight, bias, and get output_data.
	// #TODO
	for (int i = 0; i < out_channel; i++)
	{
		output_data[i] = bias[i];
		for (int j = 0; j < in_channel; j++)
			output_data[i] += weight[i][j] * input_data[j];
	}
	/*activate function*/
	softmax(output_data);
}

double OutputLayer::backward(double* input_data, double* label) {
	/*cross entropy loss*/
	double loss = 0.0;
	for (int i = 0; i < l; ++i)
		loss -= label[i] * log(output_data[i]);

	/*backpropagate activate function*/
	softmax_bp(input_data, label);

	/*backpropagation*/
	// calculate new weights and bias #TODO
	for (int i = 0; i < out_channel; i++) {
		for (int j = 0; j < in_channel; j++)
			weight[i][j] += learningRate * delta[i] * input_data[j];// partial input / partial weight = before_input
	}
	for (int i = 0; i < out_channel; i++)
		bias[i] += learningRate * delta[i];
	return loss;
}

int OutputLayer::eval(double* x) {
	forward(x);
	return getMaxIndex(output_data);		// find the max probability which is the most likely prediction
}

class NeuralNetwork {
public:
	NeuralNetwork(int in, int out, int n_hidden_layer, int* hidden_node);
	~NeuralNetwork();

	double train(double* data, int label);
	int* eval();

private:
	int in_channel;
	int out_channel;
	int num_hidden_layer;
	int* hidden_nodes;

	HiddenLayer** layers;
	OutputLayer* last_layer;
};

NeuralNetwork::NeuralNetwork(int in, int out, int n_hidden_layer, int* hidden_node) {
	in_channel = in;
	out_channel = out;
	num_hidden_layer = n_hidden_layer;
	hidden_nodes = hidden_node;

	layers = new HiddenLayer * [num_hidden_layer];
	for (int i = 0; i < num_hidden_layer; ++i) {
		if (i == 0)
			layers[i] = new HiddenLayer(in_channel, hidden_nodes[i]);				// first hiddenlayer
		else
			layers[i] = new HiddenLayer(hidden_nodes[i - 1], hidden_nodes[i]);		// other hiddenlayers
	}
	last_layer = new OutputLayer(hidden_nodes[num_hidden_layer - 1], out_channel); 	// last layer
}

NeuralNetwork::~NeuralNetwork() {
	for (int i = 0; i < num_hidden_layer; ++i)
		delete layers[i];
	delete[] layers;
	delete last_layer;
}

double NeuralNetwork::train(double* train_data, int label) {
	/*forward*/
	for (int i = 0; i < num_hidden_layer; ++i) {
		if (i == 0)
			layers[i]->forward(train_data);
		else
			layers[i]->forward(layers[i - 1]->output_data);
	}
	last_layer->forward(layers[num_hidden_layer - 1]->output_data);

	/*backward*/
	// to calculate loss, the label need to be preprocessed as one-hot format #TODO
	double loss = last_layer->backward(layers[num_hidden_layer - 1]->output_data, makeOneHot(label));

	for (int i = num_hidden_layer - 1; i >= 1; --i) {
		if (i == num_hidden_layer - 1)
			layers[i]->backward(layers[i - 1]->output_data, last_layer->delta, last_layer->weight, last_layer->out_channel);
		else
			layers[i]->backward(layers[i - 1]->output_data, layers[i + 1]->delta, layers[i + 1]->weight, layers[i + 1]->out_channel);
	}

	if (num_hidden_layer > 1)
		layers[0]->backward(train_data, layers[1]->delta, layers[1]->weight, layers[1]->out_channel);
	else
		layers[0]->backward(train_data, last_layer->delta, last_layer->weight, last_layer->out_channel);

	return loss;
}

int* NeuralNetwork::eval() {			// run forward propagation and get the result
	int* result = new int[m];
	for (int i = 0; i < m; ++i) {
		for (int j = 0; j < num_hidden_layer; ++j) {
			if (j == 0)
				layers[j]->forward(pixel[i]);
			else
				layers[j]->forward(layers[j - 1]->output_data);
		}
		result[i] = last_layer->eval(layers[num_hidden_layer - 1]->output_data);
	}
	return result;
}

int main() {
	srand(3);				// fixed random seed. change it may be useful, or useless sometimes.
	int index_label[10], i, j;
	/*input training data*/
	// #TODO
	scanf("%d %d %d", &n, &m, &l);
	maxTrainTimes = 8 * n;
	for (i = 0; i < l; i++)scanf("%d", &index_label[i]);
	for (i = 0; i < n; i++)
	{
		int tmp;
		scanf("%d", &tmp);
		for (j = 0; j < l; j++)
			if (index_label[j] == tmp)break;
		label[i] = j;
		for (int j = 0; j < 784; j++) scanf("%lf", &pixel[i][j]);
	}
	/*std::ifstream fin;
	fin.open("tmp.txt");
	fin >> n >> m >> l;
	for (int i = 0; i < l; i++) fin >> index_label[i];
	for (int i = 0; i < n; i++) {
		int tmp;
		fin >> tmp;
		for (j = 0; j < l; j++)
			if (index_label[j] == tmp)break;
		label[i] = j;
		for (int j = 0; j < 784; j++) fin >> pixel[i][j];
	}*/
	/*define network and train*/
	NeuralNetwork myNet(28 * 28, l, num_hidden_layer, hidden_nodes);

	double accum_loss = 0.0;
	for (int i = 0; i < maxTrainTimes; ++i) {
		int iter = i % n;
		double loss = myNet.train(*(pixel + iter), label[iter]);

		accum_loss = (i == 0) ? loss : (accum_loss * 9 + loss) / 10;
		if (accum_loss < 0.0001) {
			std::cout << "stop training at " << i << std::endl;
			break;
		}
	}

	/*input test data*/
	// #TODO
	for (int i = 0; i < m; i++) {
		for (int j = 0; j < 784; j++) scanf("%lf", &pixel[i][j]);
	}
	/*for (int i = 0; i < m; i++) {
		for (int j = 0; j < 784; j++)
			fin >> pixel[i][j];
	}*/
	/*evaluation*/
	int* result = new int[m];
	result = myNet.eval();
	/*output results*/
	// #TODO
	for (int i = 0; i < m; i++) printf("%d\n", index_label[result[i]]);
	return 0;
}
