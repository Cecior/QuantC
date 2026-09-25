#include <pybind11/pybind11.h>
#include <string>
#include <stdexcept>

namespace py = pybind11;

double run_naive(const std::string &gate, size_t num_qubits, int target, int control);
double run_omp(const std::string &gate, size_t num_qubits, int target, int control);
double run_avx(const std::string &gate, size_t num_qubits, int target, int control);
double run_cuda(const std::string &gate, size_t num_qubits, int target, int control);

double run_simulation(const std::string &impl, const std::string &gate,
                      size_t num_qubits, int target, int control = -1)
{
    if (impl == "naive")
    {
        return run_naive(gate, num_qubits, target, control);
    }
    else if (impl == "omp")
    {
        return run_omp(gate, num_qubits, target, control);
    }
    else if (impl == "avx")
    {
        return run_avx(gate, num_qubits, target, control);
    }
    else if (impl == "cuda")
    {
        return run_cuda(gate, num_qubits, target, control);
    }
    else
    {
        throw std::invalid_argument("Implementation not recognized: " + impl);
    }
}

PYBIND11_MODULE(qsim_core, m)
{
    m.doc() = "C++ core of the quantum simulator";

    m.def("run", &run_simulation, "Executes specified gate with requested backend",
          py::arg("impl"),
          py::arg("gate"),
          py::arg("num_qubits"),
          py::arg("target"),
          py::arg("control") = -1);
}