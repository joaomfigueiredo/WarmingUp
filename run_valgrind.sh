#!/bin/bash
valgrind --leak-check=full ./WarmingUp -t -f1 tempcountries.csv -f2 tempcities.csv
