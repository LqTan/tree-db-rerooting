#ifndef INPUT_READER_HPP
#define INPUT_READER_HPP

#include <iostream>
#include <string>
#include <vector>
#include <utility>
#include "IndexMapper.hpp"

// Lop tiep nhan du lieu dau vao: doc so luong dinh, danh sach canh (dang
// nhan chuoi) va anh xa sang chi so nguyen lien tuc thong qua IndexMapper.
// Trich nguon ly thuyet: VNOI Wiki [9], USACO Guide [10]
//
// Ghi chu sua loi (so voi phien ban truoc): voi cay chi co N = 1 dinh,
// khong ton tai canh nao trong du lieu vao, nen khong co co che nao de
// chuong trinh biet ten cua dinh duy nhat neu chi doc qua danh sach canh.
// Ban truoc day khong xu ly rieng truong hop nay, dan den IndexMapper
// khong duoc gan bat ky nhan nao, va khi xuat ket qua chuong trinh nem
// ngoai le "Chi so noi bo vuot qua pham vi." (chuong trinh dung dot ngot).
// Ham read() duoi day bo sung buoc doc rieng cho truong hop N = 1: neu
// dong du lieu tiep theo co chua nhan dinh thi dung nhan do, neu khong
// (het du lieu) thi gan nhan mac dinh la "1" de dam bao chuong trinh luon
// xuat duoc ket qua on dinh, khong bao gio nem ngoai le do thieu du lieu.
class InputReader {
public:
    static bool read(std::istream& in, int& N,
                      IndexMapper<std::string>& mapper,
                      std::vector<std::pair<int, int>>& mapped_edges) {
        if (!(in >> N)) {
            return false;
        }

        mapped_edges.clear();
        if (N > 1) {
            mapped_edges.reserve(static_cast<size_t>(N - 1));
        }

        // Truong hop dac biet: cay mot dinh, khong co canh de suy ra ten dinh
        if (N == 1) {
            std::string only_name;
            if (in >> only_name) {
                mapper.get_or_create_index(only_name);
            } else {
                mapper.get_or_create_index(std::string("1"));
            }
            return true;
        }

        for (int i = 0; i < N - 1; ++i) {
            std::string u_name, v_name;
            if (in >> u_name >> v_name) {
                int u_idx = mapper.get_or_create_index(u_name);
                int v_idx = mapper.get_or_create_index(v_name);
                mapped_edges.push_back({u_idx, v_idx});
            }
        }
        return true;
    }
};

#endif // INPUT_READER_HPP
