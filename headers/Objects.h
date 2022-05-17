#ifndef OBJECTS_H
#define OBJECTS_H
#include "./Camera.h"
#include "./Shader.h"
#include "./buffer.h"
#include "./TimerUtil.h"
#include "./Constants.h"
#include "./ColorTransition.h"
#include "./BSTree.h"
#include <cmath>
#include <cstdlib>
#include <fstream>
#include <iostream>
#include <sstream>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <set>
#include <chrono>
#include <thread>
#include <algorithm>
#include <mutex>
#define REFLECT_DIVISOR 2
using std::chrono::high_resolution_clock;
using std::chrono::duration_cast;
using std::chrono::duration;
using std::chrono::milliseconds;
std::mutex mtx;
using namespace std;
Constants *data = new Constants();
inline void to3D(int idx, int *arr, int side_len) {
    int z = idx / (side_len * side_len);
    idx -= (z * side_len * side_len);
    int y = idx / side_len;
    int x = idx % side_len;
    arr[0] = x;
    arr[1] = y;
    arr[2] = z;
}

class Object {
  public:
    vector<vector<float>> mooreN = data->mooreN;
    vector<vector<float>> vonN = data->vonN;
    vector<vector<float>> flatMooreN = data->flatMooreN;
    vector<vector<float>> flatVonN = data->flatVonN;
    float *pos_norms = data->pos_norms;
    vector<vector<float>> NHOOD;

    unsigned int num_cubes;
    unsigned int side_len;
    unsigned int nv;
    unsigned int ni;
    vector<int> cells_vec;
    vector<unsigned int> btranslations;

    bool LIGHTING_ENABLED;
    buffer *vb;
    bufferInstanced *vbi;
    Shader *sh;
    Camera *cam;
    TimerUtil *tmr = new TimerUtil();

    vector<float> stay_alive;
    vector<float> born;
    set<float> stay_alive_set;
    set<float> born_set;
    float lifecycle;

    Object(){};

    void readRuleset(int rule) {
        int na, nb;
        string fn = "./configs/rule_set.ca", ruleset;
        fstream istr(fn.c_str());
        string dummy;
        int counter = 4 * rule;
        while (counter-- >= 0) {
            getline(istr, dummy);
        }
        istr >> na >> nb;
        stay_alive = vector<float>(na);
        born = vector<float>(nb);
        for (int i = 0; i < na; i++) {
            istr >> stay_alive[(int)(i)];
        }
        for (int i = 0; i < nb; i++) {
            istr >> born[(int)(i)];
        }
        istr >> lifecycle;
        string nh;
        istr >> nh;
        string stat;
        if(nh == "FM"){
            this->NHOOD = std::move(this->flatMooreN);
            stat = "Flat Moore";
        }
        else if(nh == "M"){
            this->NHOOD = std::move(this->mooreN);
            stat = "Moore";
        }
        else if(nh == "N"){
            this->NHOOD = std::move(this->vonN);
            stat = "vonN";
        }
        else{
            this->NHOOD = std::move(this->flatVonN);
            stat = "Flat vonN";
        }
        stay_alive_set = set<float>(stay_alive.begin(), stay_alive.end());
        born_set = set<float>(born.begin(), born.end());
    }

    void init(string vertex_shader, string fragment_shader, bool LIGHTING_ENABLED, int sl = 20, Camera *camera = 0,
              int rule = 0) {

        tmr->startTimer();
        this->num_cubes = sl * sl * sl;
        this->LIGHTING_ENABLED = LIGHTING_ENABLED;
        nv = num_cubes * 8, ni = num_cubes * 12;
        side_len = sl; // cbrt(num_cubes);
        this->cells_vec = vector<int>(num_cubes, 0);
        readRuleset(rule);

        for (int z = 0; z < side_len; z++) {
            for (int y = 0; y < side_len; y++) {
                for (int x = 0; x < side_len; x++) {
                    if (sqrt((x - side_len / 2.0f) * (x - side_len / 2.0f) +
                             (y - side_len / 2.0f) * (y - side_len / 2.0f) +
                             (z - side_len / 2.0f) * (z - side_len / 2.0f)) <= 5.0f) {
                        float curr = x + side_len * y + side_len * side_len * z;
                        // if ( rand() % 10 < 3){
                        cells_vec[(int)curr] = lifecycle;
                        btranslations.push_back((x | (y << 8) | (z << 16)));
                        // }
                    }
                }
            }
        }

        for (int y = 5; y < side_len - 5; y++) {
            float curr = (side_len / 2.0) + side_len * y + side_len * side_len * (side_len / 2.0);
            cells_vec[(int)curr] = lifecycle;
            btranslations.push_back(((unsigned int)(side_len / 2) | (y << 8) | (unsigned int)(side_len / 2) << 16));
        }

        for (int x = 5; x < side_len - 5; x++) {
            float curr = x + side_len * (side_len / 2.0) + side_len * side_len * (side_len / 2.0);
            cells_vec[(int)curr] = lifecycle;
            btranslations.push_back((x | (unsigned int)(side_len / 2) << 8 | (unsigned int)(side_len / 2) << 16));
        }

        for (int z = 5; z < side_len - 5; z++) {
            float curr = (side_len / 2.0) + side_len * (side_len / 2.0) + side_len * side_len * z;
            cells_vec[(int)curr] = lifecycle;
            btranslations.push_back(((unsigned int)(side_len / 2) | (unsigned int)(side_len / 2) << 8 | (z << 16)));
        }
        double init_render = tmr->stopTimer();
        cam = camera;
        sh = new Shader(vertex_shader.c_str(), fragment_shader.c_str());
        vb = new buffer(3 * 12, &pos_norms[0]);
        vbi = new bufferInstanced(num_cubes);
    };

    inline bool inBounds(int x, int y, int z) {
        return (x >= 0 && x < side_len && y >= 0 && y < side_len && z >= 0 && z < side_len);
    }

    void worker(int start, int end, int *next_gen_vec) {
        for (int k = start; k < end; k++) {
            int pos[3] = {0, 0, 0};
            to3D(k, pos, side_len);
            int x = pos[0], y = pos[1], z = pos[2];
            if (y > side_len / REFLECT_DIVISOR || x > side_len / REFLECT_DIVISOR || z > side_len / REFLECT_DIVISOR) {
                continue;
            }
            int curr = cells_vec[z * side_len * side_len + y * side_len + x];
            int ncount = 0;
            for (auto i : NHOOD) {
                if (inBounds(x + i[0], y + i[1], z + i[2]) &&
                    cells_vec[(int)((((z + i[2]) * side_len * side_len)) + ((y + i[1]) * side_len) + (x + i[0]))] >= 1) {
                    ncount++;
                }
            }
            if ((curr == lifecycle && stay_alive_set.find(ncount) != stay_alive_set.end()) ||
                (curr == 0 && born_set.find(ncount) != born_set.end())) {
                mtx.lock();
                next_gen_vec[z * side_len * side_len + y * side_len + x] = lifecycle;
                btranslations.push_back((x | (y << 8) | (z << 16)));
                mtx.unlock();
            } else if (curr == 0) {
                mtx.lock();
                next_gen_vec[z * side_len * side_len + y * side_len + x] = 0;
                mtx.unlock();
            } else {
                mtx.lock();
                next_gen_vec[z * side_len * side_len + y * side_len + x] =
                    cells_vec[z * side_len * side_len + y * side_len + x] - 1;
                btranslations.push_back((x | (y << 8) | (z << 16)));
                mtx.unlock();
            }
        }
    };
    
    void update() {
        this->btranslations.clear();
        vector<int> next_gen_vec(num_cubes, 0);

        int num_threads = 150;
        std::vector<std::thread> threads;
        for (int i = 0; i < num_threads; i++) {
            threads.push_back(std::thread(&Object::worker, this, i * num_cubes / (1 * num_threads),
                                          (i + 1) * num_cubes / (1 * num_threads), next_gen_vec.data()));
        }

        for (auto &t : threads) {
            t.join();
        }
        if (REFLECT_DIVISOR > 1) {
            int kk = btranslations.size();
            for (int t = 0; t < kk; t++) {
                unsigned int x = btranslations[t] & 0xFF;
                unsigned int y = (btranslations[t] >> 8) & 0xFF;
                unsigned int z = (btranslations[t] >> 16) & 0xFF;

                btranslations.push_back((x | (y << 8) | ((side_len - 1 - z) << 16)));
                btranslations.push_back((x | ((side_len - 1 - y) << 8) | (z << 16)));
                btranslations.push_back(((side_len - 1 - x) | (y << 8) | (z << 16)));
                btranslations.push_back(((side_len - 1 - x) | ((side_len - 1 - y) << 8) | (z << 16)));
                btranslations.push_back((x | ((side_len - 1 - y) << 8) | ((side_len - 1 - z) << 16)));
                btranslations.push_back(((side_len - 1 - x) | (y << 8) | ((side_len - 1 - z) << 16)));
                btranslations.push_back(((side_len - 1 - x) | ((side_len - 1 - y) << 8) | ((side_len - 1 - z) << 16)));
            }
        }
        // BST tree;
        // node* temp;
        // for(int i = 0; i < btranslations.size(); i++){
        //     temp = new node;
        //     temp->key_value = btranslations[(int)(i)];
        //     temp->p_left = temp->p_right = NULL;
        //     tree.insert(tree.root, temp);
        // }
        // btranslations = tree.getArray();
        std::swap(cells_vec, next_gen_vec);
    };

    std::vector<float> red_colorByDist(){
        Color c();
        float w = (0.75) * (side_len - 2 * sqrt(3));
        int n = side_len;
        std::vector<float>red(n);
        for(float i = 0; i < n; i++){
            red[(int)(i)] = Color::B(2 * ((4 / w) * i - 2));
            printf("RED: %f, %f\n", i, red[(int)(i)]);
        }
        return red;
    }

    std::vector<float> green_colorByDist(){
        Color c();
        float w = (0.75) * (side_len - 2 * sqrt(3));
        int n = side_len;
        std::vector<float> green(n);
        for(float i = 0; i < n; i++){
            green[(int)(i)] = Color::L(2 * ((4 / w) * i - 3));
            printf("GREEN: %f, %f\n", i, green[(int)(i)]);
        }
        return green;
    }

    std::vector<float> blue_colorByDist(){
        Color c();
        float w = (0.75) * (side_len - 2 * sqrt(3));
        int n = side_len;
        std::vector<float> blue(n);
        for(float i = 0; i < n; i++){
            blue[(int)(i)] = Color::L(-2 * ((4 / w) * i - 1));
            printf("BLUE: %f, %f\n", i, blue[(int)(i)]);
        }
        return blue;
    }
    void draw() {

        sh->use();
        sh->setMat4("pvm", cam->pvm());
        sh->setMat4("model", cam->getModel());
        sh->setVec3("eye", cam->eye.x, cam->eye.y, cam->eye.z);
        sh->setFloat("side_len", 0.5f * side_len);
        
        int n = side_len;
        float a[n], b[n], c[n];
        std::vector<float> _a = red_colorByDist();
        std::vector<float> _b = green_colorByDist();
        std::vector<float> _c = blue_colorByDist();
        for(int i = 0; i < n; i++){
            a[(int)(i)] = _a[(int)(i)];
            b[(int)(i)] = _b[(int)(i)];
            c[(int)(i)] = _c[(int)(i)];
            printf("%f %f %f\n", a[(int)(i)], b[(int)(i)], c[(int)(i)]);
        }
        
        sh->set1fv("redCol",   a,   n);
        sh->set1fv("blueCol",  b,  n);
        sh->set1fv("greenCol", c, n);
        vb->use();
        glBufferSubData(GL_ARRAY_BUFFER, 0, btranslations.size() * sizeof(unsigned int), btranslations.data());
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        glDrawArraysInstanced(GL_TRIANGLES, 0, 3 * 12, btranslations.size());
    }
};

#endif