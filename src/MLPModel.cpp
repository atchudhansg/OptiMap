#include "MLPModel.hpp"
#include <cmath>
#include <cstdlib>
#include <ctime>

static double randDouble(double minVal, double maxVal) {
    double r = static_cast<double>(rand()) / RAND_MAX;
    return minVal + r * (maxVal - minVal);
}

MLPModel::MLPModel(size_t inputSize, size_t hiddenSize)
    : m_inputSize(inputSize),
      m_hiddenSize(hiddenSize),
      hiddenWeights(hiddenSize, std::vector<double>(inputSize, 0.0)),
      hiddenBias(hiddenSize, 0.0),
      outputWeights(hiddenSize, 0.0),
      outputBias(0.0)
{
    static bool seeded = false;
    if (!seeded) {
        srand(static_cast<unsigned>(time(nullptr)));
        seeded = true;
    }
    initializeWeights();
}

void MLPModel::initializeWeights() {
    // Initialize weights/bias in [-0.1, 0.1]
    for (size_t h = 0; h < m_hiddenSize; ++h) {
        for (size_t i = 0; i < m_inputSize; ++i) {
            hiddenWeights[h][i] = randDouble(-0.1, 0.1);
        }
        hiddenBias[h] = randDouble(-0.1, 0.1);
        outputWeights[h] = randDouble(-0.1, 0.1);
    }
    outputBias = randDouble(-0.1, 0.1);
}

std::pair<std::vector<double>, double> MLPModel::forward(const std::vector<int>& features) const {
    // 1) Hidden layer
    std::vector<double> hiddenOut(m_hiddenSize, 0.0);
    for (size_t h = 0; h < m_hiddenSize; ++h) {
        double sum = hiddenBias[h];
        for (size_t i = 0; i < m_inputSize; ++i) {
            sum += hiddenWeights[h][i] * static_cast<double>(features[i]);
        }
        hiddenOut[h] = relu(sum);
    }
    // 2) Output
    double out = outputBias;
    for (size_t h = 0; h < m_hiddenSize; ++h) {
        out += outputWeights[h] * hiddenOut[h];
    }
    return { hiddenOut, out };
}

double MLPModel::predict(const std::vector<int>& features) const {
    if (features.size() != m_inputSize) {
        // Dimension mismatch => return 0 or handle gracefully
        return 0.0;
    }
    auto [hidden, out] = forward(features);
    return out;
}

void MLPModel::update(const std::vector<int>& features, double actualCost) {
    if (features.size() != m_inputSize) return;

    auto [hiddenOut, pred] = forward(features);
    backward(features, hiddenOut, pred, actualCost);
}

void MLPModel::backward(const std::vector<int>& features,
                        const std::vector<double>& hiddenOut,
                        double pred,
                        double actual)
{
    // Mean squared error derivative for single sample:
    // dLoss/dPred = (pred - actual)
    double error = (pred - actual);
    double lr = 0.00001; // learning rate

    // Update output layer
    for (size_t h = 0; h < m_hiddenSize; ++h) {
        double grad = error * hiddenOut[h]; // dPred/dOutputWeight = hiddenOut[h]
        outputWeights[h] -= lr * grad;
    }
    outputBias -= lr * error;

    // Update hidden layer
    for (size_t h = 0; h < m_hiddenSize; ++h) {
        double dLoss_dHidden = error * outputWeights[h];
        double derivative = reluDeriv(hiddenOut[h]);
        double dLoss_dHiddenPost = dLoss_dHidden * derivative;

        for (size_t i = 0; i < m_inputSize; ++i) {
            double grad = dLoss_dHiddenPost * static_cast<double>(features[i]);
            hiddenWeights[h][i] -= lr * grad;
        }
        hiddenBias[h] -= lr * dLoss_dHiddenPost;
    }
}
