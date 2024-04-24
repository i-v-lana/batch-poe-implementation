//
// Created by Светлана Иванова on 11.04.2024.
//

#include "Experiments.h"
#include "helper.h"


std::vector<std::pair<std::vector<bigint>, std::vector<bigint> > > Experiments::get(long instances_per_exp, int exp) {
    std::vector<std::pair<std::vector<bigint>, std::vector<bigint> > > ans = {};
    for (int i = 0; i < exp; ++i) {
        ans.emplace_back();
        for (int j = 0; j < instances_per_exp; ++j) {
            int cur_ind = index(generator);
            ans.back().first.push_back(x[cur_ind]);
            ans.back().second.push_back(y[cur_ind]);
        }
    }
    return ans;
}

Experiments::Experiments(int sample_size, bigint& mod_N, std::pair<bigint, bigint> _trapdoor, long long _t) {
    N = mod_N;
    set_trapdoor(_trapdoor);
    t = _t;
    helper = mpz_helper();

    index = std::uniform_int_distribution<int>(0, sample_size-1);
    generator = std::default_random_engine(rd());
    for (int i = 0; i < sample_size; ++i) {
        bigint _x = helper.get_random_mod(N);
        bigint _y = trapdoor(_x);
        x.push_back(_x);
        y.push_back(_y);
    }
}

void Experiments::set_trapdoor(std::pair<bigint, bigint> _trapdoor) {
    if (N == _trapdoor.first * _trapdoor.second) {
        p = _trapdoor.first;
        q = _trapdoor.second;
        trapdoor_flag = true;
    } else {
        trapdoor_flag = false;
        print_error("EXPERIMENTS: trapdoor wasn't set");
    }
}

bigint Experiments::trapdoor(bigint &_x) {
    if (!trapdoor_flag) {
        print_error("EXPERIMENTS: Can't calculate the trapdoor, as it wasn't correctly set");
        return _x;
    }
    if (!check_trapdoor(_x)) {
        print_error("EXPERIMENTS: input for trapdoor function isn't coprime with module");
        return _x;
    }
    /// N = pq -> phi = (p - 1) * (q - 1)
    bigint phi = (p - 1UL) * (q - 1UL);
    /// counting 2^t mod phi
    bigint two(2);
    bigint big_t(t);
    bigint power = helper.pow(two, big_t, phi);
    bigint ans = helper.pow(_x, power, N);
    return ans;
}

bool Experiments::check_trapdoor(bigint &_x) {
    bigint gcd = helper.gcd(_x, N);
    return (gcd == bigint(1));
}


