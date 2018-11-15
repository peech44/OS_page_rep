// os_last.cpp : Defines the entry point for the console application.
//code by coolmost

#include "stdafx.h"
#include <iostream> 
#include <vector>
#include<unordered_set>
#include <queue>
#include <unordered_map>
using namespace std;


int pageFaults_Fifo(int pages[], int n, int capacity);
int pageFaults_Optimal(int pages[], int n, int capacity);
bool search(int key, vector<int>& fr);
int predict(int pg[], vector<int>& fr, int pn, int index);
int pageFaults_Lru(int pages[], int n, int capacity);
// for avoid spagetti code i should separate function into 5 function
// use int function to return value of page fault
int main()
{
	//use dynamic array to ask user input vaarriable
	cout << "retutn to hello word" << endl;
	
	int size;
	int input_page;
	int n;
	int capacity;
	cout << "please input size of reference string " << endl;
	cin >> size;
	cout << endl;
	n = size;
	int *pages;
	pages = new int[size];
	cout << "please input page " << endl;
	for (int i = 0; i < size; i++) {
		cin >> input_page;
		pages[i] = input_page;
	}
	cout << " please input capacity of frame " << endl;
	cin >> capacity;
	cout << endl;
	cout << "Page fault of FIFO = " << pageFaults_Fifo(pages, n, capacity) << endl;
	cout << "Page fault of Optimal = " << pageFaults_Optimal(pages, n, capacity) << endl;
	cout << " Page fault of LRU = " << pageFaults_Lru(pages, n, capacity) << endl;


	cout << endl;
	system("pause");
    return 0;
}

int pageFaults_Fifo(int pages[], int n, int capacity)
{
	// To represent set of current pages. We use 
	// an unordered_set so that we quickly check 
	// if a page is present in set or not 
	
	unordered_set<int> s;

	// To store the pages in FIFO manner 
	queue<int> indexes;

	// Start from initial page 
	int page_faults = 0;
	for (int i = 0; i<n; i++)
	{
		// Check if the set can hold more pages 
		if (s.size() < capacity)
		{
			// Insert it into set if not present 
			// already which represents page fault 
			if (s.find(pages[i]) == s.end())
			{
				s.insert(pages[i]);

				// increment page fault 
				page_faults++;

				// Push the current page into the queue 
				indexes.push(pages[i]);
			}
		}

		// If the set is full then need to perform FIFO 
		// i.e. remove the first page of the queue from 
		// set and queue both and insert the current page 
		else
		{
			// Check if current page is not already 
			// present in the set 
			if (s.find(pages[i]) == s.end())
			{
				//Pop the first page from the queue 
				int val = indexes.front();

				indexes.pop();

				// Remove the indexes page 
				s.erase(val);

				// insert the current page 
				s.insert(pages[i]);

				// push the current page into 
				// the queue 
				indexes.push(pages[i]);

				// Increment page faults 
				page_faults++;
			}
		}
	}

	return page_faults;
}

int pageFaults_Optimal(int pages[], int n, int capacity) {

	// Create an array for given number of 
	// frames and initialize it as empty. 
	vector<int> fr;

	// Traverse through page reference array 
	// and check for miss and hit. 
	int hit = 0;
	for (int i = 0; i < n; i++) {

		// Page found in a frame : HIT 
		if (search(pages[i], fr)) {
			hit++;
			continue;
		}

		// Page not found in a frame : MISS 

		// If there is space available in frames. 
		if (fr.size() < capacity)
			fr.push_back(pages[i]);

		// Find the page to be replaced. 
		else {
			int j = predict(pages, fr, n, i + 1);
			fr[j] = pages[i];
		}
	}
	//No. of hits = hit 
	//No. of misses = n - hit 
	int miss = n - hit;
	return miss;
	
}

bool search(int key, vector<int>& fr) {

	for (int i = 0; i < fr.size(); i++)
		if (fr[i] == key)
			return true;
	return false;

}

int predict(int pg[], vector<int>& fr, int pn, int index) {

	// Store the index of pages which are going 
	// to be used recently in future 
	int res = -1, farthest = index;
	for (int i = 0; i < fr.size(); i++) {
		int j;
		for (j = index; j < pn; j++) {
			if (fr[i] == pg[j]) {
				if (j > farthest) {
					farthest = j;
					res = i;
				}
				break;
			}
		}

		// If a page is never referenced in future, 
		// return it. 
		if (j == pn)
			return i;
	}

	// If all of the frames were not in future, 
	// return any of them, we return 0. Otherwise 
	// we return res. 
	return (res == -1) ? 0 : res;

}

int pageFaults_Lru(int pages[], int n, int capacity) {

	// To represent set of current pages. We use 
	// an unordered_set so that we quickly check 
	// if a page is present in set or not 
	unordered_set<int> s;

	// To store least recently used indexes 
	// of pages. 
	unordered_map<int, int> indexes;

	// Start from initial page 
	int page_faults = 0;
	for (int i = 0; i<n; i++)
	{
		// Check if the set can hold more pages 
		if (s.size() < capacity)
		{
			// Insert it into set if not present 
			// already which represents page fault 
			if (s.find(pages[i]) == s.end())
			{
				s.insert(pages[i]);

				// increment page fault 
				page_faults++;
			}

			// Store the recently used index of 
			// each page 
			indexes[pages[i]] = i;
		}

		// If the set is full then need to perform lru 
		// i.e. remove the least recently used page 
		// and insert the current page 
		else
		{
			// Check if current page is not already 
			// present in the set 
			if (s.find(pages[i]) == s.end())
			{
				// Find the least recently used pages 
				// that is present in the set 
				int lru = INT_MAX, val;
				for (auto it = s.begin(); it != s.end(); it++)
				{
					if (indexes[*it] < lru)
					{
						lru = indexes[*it];
						val = *it;
					}
				}

				// Remove the indexes page 
				s.erase(val);

				// insert the current page 
				s.insert(pages[i]);

				// Increment page faults 
				page_faults++;
			}

			// Update the current page index 
			indexes[pages[i]] = i;
		}
	}

	return page_faults;


}
