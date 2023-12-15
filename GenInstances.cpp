//
// Created by Светлана Иванова on 15.12.2023.
//

#include "GenInstances.h"
#include <fstream>

void GenInstances::gen(long long cnt, std::string file_name) {
    file_name = "t_" + std::to_string(t) + "_" + file_name;
    std::ofstream text_file(file_name, std::ofstream::out);
    for (long long i = 0; i < cnt; ++i) {
        bigint x = helper.get_random_mod(N);
        bigint y = trapdoor(x);
        text_file << x << " " << y << std::endl;
    }
    text_file.close();
}

bigint GenInstances::trapdoor(bigint& _x) {
    /// N = pq -> phi = (p - 1) * (q - 1)
    bigint phi = (p - 1UL) * (q - 1UL);
    /// counting 2^t mod phi
    bigint power = helper.pow(bigint(2), bigint(t), phi);
    bigint ans = helper.pow(_x, power, N);
    return ans;
}

void GenInstances::set_trapdoor(bigint& _p, bigint& _q) {
    if (N == _p * _q) {
        p = _p;
        q = _q;
    } else {
        /// TODO: show error
        std::cout << "GenInstances: trapdoor wasn't set." << std::endl;
    }
}

GenInstances::GenInstances(bigint _N, std::pair<bigint, bigint> _trapdoor, long _t) {
    helper = mpz_helper();
    N = _N;
    set_trapdoor(_trapdoor.first, _trapdoor.second);
    t = _t;
}

std::pair<std::vector<bigint>, std::vector<bigint> > GenInstances::get_instances(std::string file_name, long long cnt) {
    /// TODO: fix the random choice of instances.
    std::vector<bigint> x(0), y(0), cur_x(0), cur_y(0);
    file_name = "t_" + std::to_string(t) + "_" + file_name;
    std::ifstream text_file(file_name, std::ifstream::in);

    std::string str;
    long long i = 0;
    while (text_file >> str) {
        if (i % 2 == 0) {
            cur_x.push_back(bigint(str));
        } else {
            cur_y.push_back(bigint(str));
        }
        ++i;
    }
    for (i = 0; i < cnt; ++i) {
        x.push_back(cur_x[i]);
        y.push_back(cur_y[i]);
    }
    return {x, y};
}