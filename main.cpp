#include <map>
#include <vector>
#include <string>
#include <iostream>

struct TransactionRecord
{
 std::wstring m_accountName;
 std::wstring m_service;
 std::wstring m_api;
 int transactions;
 int ingress;
 int egress;

 TransactionRecord()
 : m_accountName(L""),
 m_service(L""),
 m_api(L""),
 transactions(0),
 ingress(0),
 egress(0)
 {
 }

 TransactionRecord(std::wstring acc, std::wstring srvc, std::wstring api, int t, int i, int e)
 : m_accountName(acc),
 m_service(srvc),
 m_api(api),
 transactions(t),
 ingress(i),
 egress(e)
 {
 }

 std::wstring ToString()
 {
  return m_accountName + L"_" + m_service + L"_" + m_api + L"_" + std::to_wstring(transactions) + L"_" + std::to_wstring(ingress) + L"_" + std::to_wstring(egress);
 }
};

class IAggregator
{
public:
virtual std::wstring GetAggregationKey(TransactionRecord tr) = 0;

 void AddRecord(TransactionRecord tr)
{
  const auto& key = GetAggregationKey(tr);
  auto it = m_result.find(key);
  if(it == m_result.end())
  {
   m_result[key] = tr;
  }
  else
  {
   m_result[key].transactions += tr.transactions;
   m_result[key].ingress += tr.ingress;
   m_result[key].egress += tr.egress;
  }
}

 void print()
{
  for(auto it = m_result.begin(); it != m_result.end(); it++)
  {
   printf("%ls ==> %ls\n", it->first.c_str(), it->second.ToString().c_str());
  }
  printf("=================================");
}

 std::map<std::wstring, TransactionRecord> GetResult()
{
  return m_result;
}

 /* void clear() */

 protected:
  std::map<std::wstring, TransactionRecord> m_result;
};

class AccountLevelAggregator : public IAggregator
{
public:
std::wstring GetAggregationKey(TransactionRecord tr)
{
     return L"Total";
}
};

class ServiceLevelAggregator : public IAggregator
{
public:
std::wstring GetAggregationKey(TransactionRecord tr)
{
     return tr.m_service;
}
};

class ApiLevelAggregator : public IAggregator
{
public:
std::wstring GetAggregationKey(TransactionRecord tr)
{
     return tr.m_service + L"_" + tr.m_api;
}
};

class TransactionEngine
{
public:
std::vector<IAggregator> m_aggregators;

 void AddRecord(TransactionRecord tr)
{
  for(auto aggregator: m_aggregators)
  {
   aggregator.AddRecord(tr);
  }
}

 void print()
{
  for(auto const& aggregator: m_aggregators)
  {
   aggregator.print();
  }
 }
};

int main()
{
//TransactionRecord tr(L"acc", L"blob", L"getblob", 1, 2, 3);
//printf("%ls", tr.ToString().c_str());

 TransactionEngine tEngine;
tEngine.m_aggregators.push_back(ApiLevelAggregator());
tEngine.m_aggregators.push_back(ServiceLevelAggregator());
tEngine.m_aggregators.push_back(AccountLevelAggregator());

 tEngine.AddRecord(TransactionRecord(L"acc", L"blob", L"getblob", 1, 2, 3));
tEngine.AddRecord(TransactionRecord(L"acc", L"blob", L"getblob", 3, 4, 5));
tEngine.AddRecord(TransactionRecord(L"acc", L"blob", L"putblob", 3, 4, 5));

 tEngine.print();

}
