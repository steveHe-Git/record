#ifndef _ROUTE_MANAGER_
#define _ROUTE_MANAGER_

#include <iostream>
#include <unordered_map>
#include <vector>
#include <memory>

using namespace std;

typedef struct Route {
    int id;
    std::string start_point;
    std::string end_point;
    double length;

    inline bool operator==(const Route& data) const{
        if (data.id == id &&
            data.start_point == start_point &&
            data.end_point == end_point && 
            data.length == length) {
            return true;
        } else {
            return false;
        }
    }

    void printData() {
        cout << "Id: " << id
            << " start: " << start_point
            << " end: " << end_point
            << " len: " << length << endl;
    }
}RouteData;



class RouteManger
{
private:
    /* data */

    using RouteDataPtr = std::shared_ptr<RouteData>;

    unordered_map<int, RouteDataPtr> m_map;
public:
    RouteManger(/* args */){};
    ~RouteManger(){};

    int update(int id,  const string& start_point, const string& end_point, double length){
        auto dataPtr = std::make_shared<RouteData>(RouteData{id, start_point, end_point, length});
        auto ret = m_map.find(id);
        if (ret != m_map.end()) {
            dataPtr->id = id;
            ret->second = dataPtr;
            return ret->first;
        } else {
            dataPtr->id = m_map.size() + 1;
            auto result = m_map.insert(std::pair<int, RouteDataPtr>(m_map.size() + 1 , dataPtr));
            return result.first->first;
        }
    }

    const Route& find_by_id(int id) {
        auto ret = m_map.find(id);
        if (ret != m_map.end()) {
            return *(ret->second);
        }
        throw "not found route by id!";
    }

    vector<int> find_by_start_point(const std::string& start_point) {
        std:vector<int> tmp;
        for(auto& element: m_map) {
            //cout << element.second->id << endl;
            if (element.second->start_point == start_point) {
                tmp.push_back(element.second->id);
            }
        }
        return tmp;
    }

    vector<int> find_cycle_route(const std::vector<std::string>& points){
        std::vector<int> tmp;
        decltype(points.size()) size = points.size();
        int distance[size][size] = {0};
        if (size > 15) {
            throw "the provided point vector is over threshold";
        } else {
            //算法实现，经过所有的points的闭环最短路径(每个点只经过一次)
            
        }
        return tmp;
    }

    decltype(m_map.size()) getSize() {
        return m_map.size();
    }

    void printMapData() {
        for (auto& data: m_map) {
            data.second->printData();
        }
        cout << endl;
    }
};
#endif