# 1 STL简介

STL(standard template library)标准模板库，是C++标准库的核心，是一个泛型的程序库，提供一系列软件方案，利用先进、高效的算法来管理数据

STL由一些可适用不同需求的集合类和一些能够在这些数据集合上运作的算法构成。STL建立一个框架，在些框架下你可以提供其他集合类或算法，与现有的组件搭配共同运作

## 1.1 STL组件

STL的基本观念就是将数据和操作分离。数据由容器类加以管理，操作则由可定制(configurable)的算法定义之。迭代器在两者之间充当黏合剂，使用任何算法都可以和任何容器交互

![STL组件之间的合作](res\STL组件之间的合作.png)

### 1.1.1 容器(Container)

用来管理某类对象的集合

### 1.1.2 迭代器(Iterator)

用来在一个对象集合(collection of objects)内遍历元素。这个对象集合可以是一个容器，或容器的一部分

### 1.1.3 算法(Algorithm)

用来处理集合内的元素。可以出于不同的目的而查找、排序、修改、使用元素。通过迭代器的协助，只需写一次算法，就可以将它们应用于任意容器。你可以提供一些特殊的辅助函数供算法调用

# 2 容器(Container)

![STL的容器种类](res\STL的容器种类.png)

## 2.1 顺序容器

sequence container: 一种有序(ordered)集合，其中每个元素均有确凿的位置(取决于插入时机和位置，与元素值无关)

array、vector、deque、list、forward_list

通常被实现为 array或 linked list

## 2.2 关联式容器

associative container: 已排序(sorted)集合，元素位置取决于其value或key和给定的某个排序准则

set、multiset、map、multimap

通常被实现为 binary tree

## 2.3 无序容器

unordered container：一种无序列集合(unordered collection)，元素的位置无关紧要，唯一重要的是某特定元素是否位于此集合内

unordered_set、unordered_multiset、unordered_map、unordered_multimap

通常被实现为hash table



