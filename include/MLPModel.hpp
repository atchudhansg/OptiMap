#pragma once
#include <vector>
#include <utility>

class MLPModel {
public:
    // Constructor: specify input size (#features), hidden layer size, etc.
    MLPModel(size_t inputSize, size_t hiddenSize = 8);

    // Predict the cost for a given feature vector
    double predict(const std::vector<int>& features) const;

    // Update the model (train on a single sample)
    void update(const std::vector<int>& features, double actualCost);

private:
    size_t m_inputSize;
    size_t m_hiddenSize;

    // hidden layer: hiddenWeights[h][i] is weight from input i -> hidden neuron h
    std::vector<std::vector<double>> hiddenWeights;
    std::vector<double> hiddenBias;

    // output layer: outputWeights[h] is weight from hidden neuron h -> single output
    std::vector<double> outputWeights;
    double outputBias;

    // Activation functions
    double relu(double x) const { return (x > 0.0) ? x : 0.0; }
    double reluDeriv(double x) const { return (x > 0.0) ? 1.0 : 0.0; }

    // Helpers
    void initializeWeights();
    std::pair<std::vector<double>, double> forward(const std::vector<int>& features) const;
    void backward(const std::vector<int>& features,
                  const std::vector<double>& hiddenOut,
                  double pred,
                  double actual);
};
