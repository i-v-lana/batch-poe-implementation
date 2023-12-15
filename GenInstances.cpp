//
// Created by Светлана Иванова on 15.12.2023.
//

#include "GenInstances.h"

void GenInstances::gen(long long cnt, std::string file_name) {
    file_name = "t_" + std::to_string(cnt) + "_" + file_name;
    std::ofstream text_file(file_name, std::ios::binary);
    for (long long i = 0; i < cnt; ++i) {
        bigint x = helper.get_random_mod(N);
        bigint y = trapdoor(x);
        text_file.write(reinterpret_cast<char*>(&bigintPairs[0]), bigintPairs.size() * sizeof(std::pair<int64_t, int64_t>));
    }
    outFile.close();
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
