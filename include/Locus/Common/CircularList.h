/********************************************************************************************************\
*                                                                                                        *
*   This file is part of the Locus Game Engine                                                           *
*                                                                                                        *
*   Copyright (c) 2014 Shachar Avni. All rights reserved.                                                *
*                                                                                                        *
*   Use of this file is governed by a BSD-style license. See the accompanying LICENSE.txt for details    *
*                                                                                                        *
\********************************************************************************************************/

#pragma once

#include <list>

namespace Locus
{

template <class T, class Allocator = std::allocator<T>>
class CircularList
{
public:
   typedef std::list<T, Allocator> mylist_t;

   typedef typename mylist_t::value_type value_type;
   typedef typename mylist_t::size_type size_type;
   typedef typename mylist_t::reference reference;
   typedef typename mylist_t::const_reference const_reference;
   typedef typename mylist_t::allocator_type allocator_type;

   template <class K, class Alloc>
   friend bool operator ==(CircularList<K, Alloc>& lhs, CircularList<K, Alloc>& rhs);

   template <class K, class Alloc>
   friend bool operator !=(CircularList<K, Alloc>& lhs, CircularList<K, Alloc>& rhs);

   template <class K, class Alloc>
   friend bool operator <(CircularList<K, Alloc>& lhs, CircularList<K, Alloc>& rhs);

   template <class K, class Alloc>
   friend bool operator <=(CircularList<K, Alloc>& lhs, CircularList<K, Alloc>& rhs);

   template <class K, class Alloc>
   friend bool operator >(CircularList<K, Alloc>& lhs, CircularList<K, Alloc>& rhs);

   template <class K, class Alloc>
   friend bool operator >=(CircularList<K, Alloc>& lhs, CircularList<K, Alloc>& rhs);

private:
   template <class IterType, class InnerType, class ListPointerType>
   class iterator_base
   {
#define ITERATOR_BASE_SELF iterator_base<IterType, InnerType, ListPointerType>

   public:
      ITERATOR_BASE_SELF (IterType innerIter, ListPointerType ownerList)
         : innerIter(innerIter), ownerList(ownerList)
      {
      }

#define DERIVED_ITERATOR_BODY(ClassType, IterType, InnerType, ListPointerType, Start, Finish) \
      ClassType(IterType innerIter, ListPointerType ownerList)\
         : iterator_base<IterType, InnerType, ListPointerType>(innerIter, ownerList)\
      {\
      }\
      ClassType previous() const\
      {\
         ClassType prev(*this);\
         if (prev.innerIter == Start)\
         {\
            prev.innerIter = Finish;\
         }\
         --prev;\
         return prev;\
      }\
      ClassType next() const\
      {\
         ClassType next(*this);\
         ++next;\
         if (next.innerIter == Finish)\
         {\
            next.innerIter = Start;\
         }\
         return next;\
      }

      ITERATOR_BASE_SELF & operator ++()
      {
         ++innerIter;
         return *this;
      }

      ITERATOR_BASE_SELF operator ++(int)
      {
         ITERATOR_BASE_SELF temp(*this);
         ++(*this);
         return temp;
      }

      ITERATOR_BASE_SELF & operator --()
      {
         --innerIter;
         return *this;
      }

      ITERATOR_BASE_SELF operator --(int)
      {
         ITERATOR_BASE_SELF temp(*this);
         --(*this);
         return temp;
      }

      InnerType * operator->()
      {
         return &(*innerIter);
      }

      InnerType * operator->() const
      {
         return &(*innerIter);
      }

      InnerType & operator*()
      {
         return *innerIter;
      }

      InnerType & operator*() const
      {
         return *innerIter;
      }

      bool operator !=(const ITERATOR_BASE_SELF & other) const
      {
         return innerIter != other.innerIter;
      }

      bool operator ==(const ITERATOR_BASE_SELF & other) const
      {
         return innerIter == other.innerIter;
      }

   protected:
      IterType innerIter;
      ListPointerType ownerList;

#undef ITERATOR_BASE_SELF
   };

public:
   class iterator : public iterator_base<typename mylist_t::iterator, T, mylist_t*>
   {
      friend class CircularList;
      friend class const_iterator;

   public:

      DERIVED_ITERATOR_BODY(iterator, typename mylist_t::iterator, T, mylist_t*, this->ownerList->begin(), this->ownerList->end())

   };

   class const_iterator : public iterator_base<typename mylist_t::const_iterator, const T, const mylist_t*>
   {
      friend class CircularList;

   public:

      DERIVED_ITERATOR_BODY(const_iterator, typename mylist_t::const_iterator, const T, const mylist_t*, this->ownerList->cbegin(), this->ownerList->cend())

      const_iterator(const iterator& iter)
         : iterator_base<typename mylist_t::const_iterator, const T, const mylist_t*>(iter.innerIter, iter.ownerList)
      {
      }

   };

   class reverse_iterator : public iterator_base<typename mylist_t::reverse_iterator, T, mylist_t*>
   {
      friend class CircularList;
      friend class const_reverse_iterator;

   public:

      DERIVED_ITERATOR_BODY(reverse_iterator, typename mylist_t::reverse_iterator, T, mylist_t*, this->ownerList->rbegin(), this->ownerList->rend())

   };

   class const_reverse_iterator : public iterator_base<typename mylist_t::const_reverse_iterator, const T, const mylist_t*>
   {
      friend class CircularList;

   public:

      DERIVED_ITERATOR_BODY(const_reverse_iterator, typename mylist_t::const_reverse_iterator, const T, const mylist_t*, this->ownerList->crbegin(), this->ownerList->crend())

      const_reverse_iterator(const reverse_iterator& iter)
         : iterator_base<typename mylist_t::const_reverse_iterator, const T, const mylist_t*>(iter.innerIter, iter.ownerList)
      {
      }

   };

#undef DERIVED_ITERATOR_BODY

   CircularList& operator=(const CircularList& other)
   {
      innerList = other.innerList;
      return *this;
   }

   CircularList& operator=(CircularList&& other)
   {
      innerList = other.innerList;
      return *this;
   }

   void assign(size_type count, const T& value)
   {
      innerList.assign(count, value);
   }

   template <class InputIt>
   void assign(InputIt first, InputIt last)
   {
      innerList.assign(first.innerIter, last.innerIter);
   }

   allocator_type get_allocator() const
   {
      return innerList.get_allocator();
   }

   iterator begin()
   {
      return iterator(innerList.begin(), &innerList);
   }

   const_iterator begin() const
   {
      return cbegin();
   }

   const_iterator cbegin() const
   {
      return const_iterator(innerList.cbegin(), &innerList);
   }

   reverse_iterator rbegin()
   {
      return reverse_iterator(innerList.rbegin(), &innerList);
   }

   const_reverse_iterator rbegin() const
   {
      return crbegin();
   }

   const_reverse_iterator crbegin() const
   {
      return const_reverse_iterator(innerList.crbegin(), &innerList);
   }

   iterator end()
   {
      return iterator(innerList.end(), &innerList);
   }

   const_iterator end() const
   {
      return cend();
   }

   const_iterator cend() const
   {
      return const_iterator(innerList.cend(), &innerList);
   }

   reverse_iterator rend()
   {
      return reverse_iterator(innerList.rend(), &innerList);
   }

   const_reverse_iterator rend() const
   {
      return crend();
   }

   const_reverse_iterator crend() const
   {
      return const_reverse_iterator(innerList.crend(), &innerList);
   }

   size_type size() const
   {
      return innerList.size();
   }

   size_type max_size() const
   {
      return innerList.max_size();
   }

   bool empty() const
   {
      return innerList.empty();
   }

   void clear()
   {
      return innerList.clear();
   }

   void resize(size_type count)
   {
      innerList.resize(count);
   }

   void resize(size_type count, const value_type& value)
   {
      innerList.resize(count, value);
   }

   void swap(CircularList& other)
   {
      innerList.swap(other);
   }

   reference front()
   {
      return innerList.front();
   }

   const_reference front() const
   {
      return innerList.front();
   }

   reference back()
   {
      return innerList.back();
   }

   const_reference back() const
   {
      return innerList.back();
   }

   void push_back(const value_type& val)
   {
      innerList.push_back(val);
   }

   void push_back(value_type&& val)
   {
      innerList.push_back(val);
   }

   void push_front(const value_type& val)
   {
      innerList.push_front(val);
   }

   void push_front(value_type&& val)
   {
      innerList.push_front(val);
   }

   void pop_back()
   {
      innerList.pop_back();
   }

   void pop_front()
   {
      innerList.pop_front();
   }

   //NOTE: Signatures of insert, erase, and splice should be updated
   //to take const_iterators when g++ supports it

   iterator insert(iterator pos, const T& value)
   {
      return iterator(innerList.insert(pos.innerIter, value), &innerList);
   }

   iterator insert(iterator pos, T&& value)
   {
      return iterator(innerList.insert(pos.innerIter, value), &innerList);
   }

   iterator insert(iterator pos, size_type count, const T& value)
   {
      return iterator(innerList.insert(pos.innerIter, count, value), &innerList);
   }

   template <class InputIt>
   iterator insert(iterator pos, InputIt first, InputIt last)
   {
      return iterator(innerList.insert(pos.innerIter, first.innerIter, last.innerIter), &innerList);
   }

   iterator erase(iterator pos)
   {
      return iterator(innerList.erase(pos.innerIter), &innerList);
   }

   iterator erase(iterator first, iterator last)
   {
      return iterator(innerList.erase(first.innerIter, last.innerIter), &innerList);
   }

   void merge(CircularList& other)
   {
      innerList.merge(other);
   }

   void merge(CircularList&& other)
   {
      innerList.merge(other);
   }

   template <class Compare> 
   void merge(CircularList& other, Compare comp)
   {
      innerList.merge(other, comp);
   }

   template <class Compare> 
   void merge(CircularList&& other, Compare comp)
   {
      innerList.merge(std::move(other.innerList), comp);
   }

   void splice(iterator pos, CircularList& other)
   {
      innerList.splice(pos.innerIter, other.innerList);
   }

   void splice(iterator pos, CircularList&& other)
   {
      innerList.splice(pos.innerIter, std::move(other.innerList));
   }

   void splice(iterator pos, CircularList& other, iterator it)
   {
      innerList.splice(pos.innerIter, other.innerList, it.innerIter);
   }

   void splice(iterator pos, CircularList&& other, iterator it)
   {
      innerList.splice(pos.innerIter, std::move(other.innerList), it.innerIter);
   }

   void splice(iterator pos, CircularList& other, iterator first, iterator last)
   {
      innerList.splice(pos.innerIter, other.innerList, first.innerIter, last.innerIter);
   }

   void splice(iterator pos, CircularList&& other, iterator first, iterator last)
   {
      innerList.splice(pos.innerIter, std::move(other.innerList), first.innerIter, last.innerIter);
   }

   void remove(const T& value)
   {
      innerList.remove(value);
   }

   template <class UnaryPredicate>
   void remove_if(UnaryPredicate p)
   {
      innerList.remove_if(p);
   }

   void reverse()
   {
      innerList.reverse();
   }

   void unique()
   {
      innerList.unique();
   }

   template <class BinaryPredicate>
   void unique(BinaryPredicate p)
   {
      innerList.unique(p);
   }

   void sort()
   {
      innerList.sort();
   }

   template <class Compare> 
   void sort(Compare comp)
   {
      innerList.sort(comp);
   }

private:
   mylist_t innerList;
};

template <class T, class Allocator>
bool operator ==(Locus::CircularList<T, Allocator>& lhs, CircularList<T, Allocator>& rhs)
{
   return lhs.innerList == rhs.innerList;
}

template <class T, class Allocator>
bool operator !=(CircularList<T, Allocator>& lhs, CircularList<T, Allocator>& rhs)
{
   return lhs.innerList != rhs.innerList;
}

template <class T, class Allocator>
bool operator <(CircularList<T, Allocator>& lhs, CircularList<T, Allocator>& rhs)
{
   return lhs.innerList < rhs.innerList;
}

template <class T, class Allocator>
bool operator <=(CircularList<T, Allocator>& lhs, CircularList<T, Allocator>& rhs)
{
   return lhs.innerList <= rhs.innerList;
}

template <class T, class Allocator>
bool operator >(CircularList<T, Allocator>& lhs, CircularList<T, Allocator>& rhs)
{
   return lhs.innerList > rhs.innerList;
}

template <class T, class Allocator>
bool operator >=(CircularList<T, Allocator>& lhs, CircularList<T, Allocator>& rhs)
{
   return lhs.innerList >= rhs.innerList;
}

}

namespace std
{

template <class T, class Allocator>
void swap(Locus::CircularList<T, Allocator>& lhs, Locus::CircularList<T, Allocator>& rhs)
{
   lhs.swap(rhs);
}

}