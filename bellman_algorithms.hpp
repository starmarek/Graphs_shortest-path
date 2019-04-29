#pragma once

#include <chrono>

#include "list_graph.hpp"
#include "matrix_graph.hpp"


double bellmanFord(std::shared_ptr<ListGraph> t_graph, int t_startNode, bool t_printSolution);

double bellmanFord(std::shared_ptr<MatrixGraph> t_graph, int t_startNode, bool t_printSolution);