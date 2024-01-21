#include <vector>
#include <stdexcept>
#include <iostream>
#include <cmath>
#include <iomanip>  
using namespace std;

class Integral {
public:
    Integral() : argumentValues(nullptr), functionValues(nullptr), n(0) {}

    Integral(size_t size, const vector<double>& args, const vector<double>& vals)
        : n(size)
    {
        if (args.size() != size || vals.size() != size) {
            throw invalid_argument("Size mismatch between argument values and function values.");
        }

        argumentValues = new double[size];
        functionValues = new double[size];

        if (!argumentValues || !functionValues) {
            throw bad_alloc();
        }

        copy(begin(args), end(args), argumentValues);
        copy(begin(vals), end(vals), functionValues);
    }

    Integral(const Integral& other) : n(other.n) {
        argumentValues = new double[n];
        functionValues = new double[n];

        if (!argumentValues || !functionValues) {
            throw bad_alloc();
        }

        copy(other.argumentValues, other.argumentValues + n, argumentValues);
        copy(other.functionValues, other.functionValues + n, functionValues);
    }

    Integral& operator=(const Integral& other) {
        if (this == &other) return *this;
        delete[] argumentValues;
        delete[] functionValues;
        argumentValues = nullptr;
        functionValues = nullptr;
        n = other.n;

        argumentValues = new double[n];
        functionValues = new double[n];

        if (!argumentValues || !functionValues) {
            throw bad_alloc();
        }

        copy(other.argumentValues, other.argumentValues + n, argumentValues);
        copy(other.functionValues, other.functionValues + n, functionValues);
        return *this;
    }

    ~Integral() {
        delete[] argumentValues;
        delete[] functionValues;
    }

    double operator[](size_t index) const {
        if (index >= n) {
            throw out_of_range("Index out of range");
        }
        return functionValues[index];
    }

    double trapezoidalMethod() const {
        double integral = 0.0;
        for (size_t i = 1; i < n; ++i) {
            integral += 0.5 * (functionValues[i] + functionValues[i - 1]) * (argumentValues[i] - argumentValues[i - 1]);
        }
        return integral;
    }

    double SimpsonMethod() const {
        if (n % 2 == 0) {
            throw logic_error("The number of intervals must be odd for Simpson's Method.");
        }

        double h = (argumentValues[n - 1] - argumentValues[0]) / (n - 1);
        double integral = functionValues[0] + functionValues[n - 1];

        for (size_t i = 1; i < n - 1; ++i) {
            int coef = (i & 1) ? 4 : 2;  // alternates 4/2/4/2/...
            integral += coef * functionValues[i];
        }

        integral *= h / 3;
        return integral;
    }

    double leftRectangleMethod() const {
        double integral = 0.0;
        for (size_t i = 0; i < n - 1; ++i) {
            integral += functionValues[i] * (argumentValues[i + 1] - argumentValues[i]);
        }
        return integral;
    }

    double middleRectangleMethod() const {

        double integral = 0.0;
        for (size_t i = 0; i < n - 1; ++i) {
            double midArg = (argumentValues[i] + argumentValues[i + 1]) / 2;
            double midfVal = (functionValues[i] + functionValues[i + 1]) / 2;

            integral += midfVal * (argumentValues[i + 1] - argumentValues[i]);
        }
        return integral;
    }

    double rightRectangleMethod() const {
        double integral = 0.0;
        for (size_t i = 1; i < n; ++i) {
            integral += functionValues[i] * (argumentValues[i] - argumentValues[i - 1]);
        }
        return integral;
    }

    double newtonMethod() const {
        if (n < 4 || n % 3 != 1) {
            throw invalid_argument("Invalid number of points for Newton's 3/8 rule. It must satisfy the condition: (number_of_points - 1) % 3 == 0.");
        }
        double sum = 0.0;
        for (int i = 0; i < n - 3; i += 3) {
            double h = (argumentValues[i + 3] - argumentValues[i]) / 3;
            sum += (functionValues[i] + 3 * functionValues[i + 1] + 3 * functionValues[i + 2] + functionValues[i + 3]) * 3 * h / 8;
        }
        return sum;
    }

private:
    double* argumentValues;
    double* functionValues;
    size_t n;
    friend ostream& operator<<(ostream& os, const Integral& integral);
};

ostream& operator<<(ostream& os, const Integral& integral) {

    os << "input= argument ";
    for (size_t i = 0; i < integral.n; ++i) {
        os << integral.argumentValues[i] << (i < integral.n - 1 ? "  " : "\n");
    }


    os << "function ";
    for (size_t i = 0; i < integral.n; ++i) {
        os << integral.functionValues[i] << (i < integral.n - 1 ? " " : "\n");
    }
    return os;
}

int main() {
    try {
        size_t size;
        cin >> size;

        vector<double> argumentValues(size);
        vector<double> functionValues(size);

        for (double& val : argumentValues) {
            cin >> val;
        }

        for (double& val : functionValues) {
            cin >> val;
        }


        Integral integral(size, argumentValues, functionValues);

        cout << integral;

        cout << "lev priam= " << round(integral.leftRectangleMethod() * 10) / 10 << endl;
        cout << "sr priam= " << round(integral.middleRectangleMethod() * 10) / 10 << endl;
        cout << "prav priam= " << round(integral.rightRectangleMethod() * 10) / 10 << endl;
        cout << "trapeciy= " << round(integral.trapezoidalMethod() * 10) / 10 << endl;
        cout << "Simpson= " << round(integral.SimpsonMethod() * 10) / 10 << endl;
        cout << "Newton " << round(integral.newtonMethod() * 10) / 10 << endl;
    }
    catch (const bad_alloc& e) {
        cerr << "Memory allocation error: " << e.what() << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}
