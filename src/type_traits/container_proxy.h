//////////////////////////////////////////////////////////////////////////////////////
// This file is distributed under the University of Illinois/NCSA Open Source License.
// See LICENSE file in top directory for details.
//
// Copyright (c) 2016 Jeongnim Kim and QMCPACK developers.
//
// File developed by: Jeremy McMinnis, jmcminis@gmail.com, University of Illinois at Urbana-Champaign
//                    Jeongnim Kim, jeongnim.kim@gmail.com, University of Illinois at Urbana-Champaign
//                    Mark A. Berrill, berrillma@ornl.gov, Oak Ridge National Laboratory
//
// File created by: Jeongnim Kim, jeongnim.kim@gmail.com, University of Illinois at Urbana-Champaign
//////////////////////////////////////////////////////////////////////////////////////
    
    


#ifndef QMCPLUSPLUS_MPI_CONTAINER_PROXY_H
#define QMCPLUSPLUS_MPI_CONTAINER_PROXY_H

#include <type_traits/scalar_traits.h>

namespace qmcplusplus
{

template<typename T>
struct container_proxy
{
  enum {DIM=scalar_traits<T>::DIM};
  typedef typename scalar_traits<T>::real_type* pointer;
  T& ref;
  inline container_proxy(T& a):ref(a) {}
  inline size_t size() const
  {
    return DIM;
  }
  inline pointer data()
  {
    return scalar_traits<T>::get_address(&ref);
  }
};

template<typename T>
struct container_proxy<std::vector<T> >
{
  enum {DIM=scalar_traits<T>::DIM};
  typedef typename container_proxy<T>::pointer pointer;
  std::vector<T>& ref;
  inline container_proxy(std::vector<T>& a):ref(a) {}
  inline size_t size() const
  {
    return ref.size()*container_proxy<T>::DIM;
  }
  inline pointer data()
  {
    return scalar_traits<T>::get_address(&ref[0]);
  }

  inline void resize(size_t n)
  {
    ref.resize(n);
  }

  template<typename I>
  inline void resize(I* n, int d)
  {
    size_t nt=n[0];
    for(int i=1;i<d;++i) nt *=n[i];
    ref.resize(nt);
  }
};

template<>
struct container_proxy<std::vector<bool> >
{
  enum {DIM=1};
  typedef int* pointer;
  std::vector<bool>& ref;
  std::vector<int> my_copy;
  inline container_proxy(std::vector<bool>& a):ref(a)
  {
    my_copy.resize(a.size());
    copy(a.begin(),a.end(),my_copy.begin());
  }
  ~container_proxy()
  {
    copy(my_copy.begin(),my_copy.end(),ref.begin());
  }
  inline size_t size() const
  {
    return my_copy.size();
  }
  inline pointer data()
  {
    return &my_copy[0];
  }
};


}
#endif
