#include <iostream>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

int main(int argc, char * argv[]) {
	if(argc < 2) {
		cerr << "Please supply a filename." << endl;
		return 1;
	}
	ifstream inFile;
	inFile.open(argv[1]);
	string seq0, seq1;
	string across;
	string down;
	inFile >> seq0 >> across >> seq1 >> down;
	int alen = across.length();
	across = "_" + across;
	down = "_" + down;

	vector<vector<int> > grid;
	for(size_t i = 0; i < across.size(); i++) {
		vector<int> column;
		column.push_back(0);
		grid.push_back(column);
	}

	for(size_t i = 1; i < down.size(); i++) {
		grid[0].push_back(0);
	}
	int bestScorex = 0;
	int bestScorey = 0;

	for(size_t x = 1; x < across.size(); x++) {
		for(size_t y = 1; y < down.size(); y++) {
			int up = grid[x][y-1];
			int left = grid[x-1][y];
			int diag = grid[x-1][y-1];

			if(up >= 2) {
				up -= 2;
			} else {
				up = 0;
			}

			if(left >= 2) {
				left -= 2;
			} else {
				left = 0;
			}

			if(across[x] != down[y]) {
				if(diag >= 3) {
					diag -= 3;
				} else {
					diag = 0;
				}
			} else {
				diag += 3;
			}

			grid[x].push_back(up);

			if(grid[x][y] < left) {
				grid[x][y] = left;
			}

			if(grid[x][y] < diag) {
				grid[x][y] = diag;
			}

			if(grid[x][y] > grid[bestScorex][bestScorey]) {
				bestScorex = x;
				bestScorey = y;
			}
		}
	}

	string fin1 = "";
	string fin2 = "";
	size_t x = bestScorex;
	size_t y = bestScorey;

	while(x > 0 && y > 0) {
		int up = grid[x][y-1];
		int left = grid[x-1][y];
		int diag = grid[x-1][y-1];
		if(diag >= up && diag >= left) {
			fin1 = across[x] + fin1;
			fin2 = down[y] + fin2;
			x--;
			y--;
		} else if(left >= up) {
			fin1 = across[x] + fin1;
			fin2 = "_" + fin2;
			x--;
		} else {
			fin1 = "_" + fin1;
			fin2 = down[y] + fin2;
			y--;
		}
		if(grid[x][y] == 0) {
			break;
		}
	}
	int numEds = 0;
	for(size_t i = 0; i < fin1.size(); i++) {
		if(fin1[i] != fin2[i]) {
			numEds++;
		}
	}

	cout << fin1 << endl;
	cout << fin2 << endl;
	ofstream results;
	results.open("results.fasta");
	results << seq0 << endl;
	results << fin1 << endl;
	results << seq1 << endl;
	results << fin2;
	results.close();
	cout << endl;

	cout << "Alignment score for region of similarity: " << numEds << endl;
	//cout << fin1.length() << " " << alen << endl;
	double a = (double)(1.0 * fin1.length());
	double b = (double)(1.0 * alen);
	//cout << a/b << endl;
	if(a/b >= 0.7){
	  cout << "Sequence 1 is largely similar to sequence 2" << endl;
	}
	else if(a/b >= 0.5){
	  cout << "Sequence 1 is slightly similar to sequence 2" << endl;
	}
	else{
	  cout << "Sequence 1 is largely dissimilar to sequence 2" << endl;
	}
	cout << endl;

	if(max(across.size(), down.size()) < 64) {
		// grid output
		cout << '\t';
		for(size_t i = 0; i < down.size(); i++) {
			cout << down[i] << '\t';
		}
		cout << endl << endl << endl;

		for(size_t x = 0; x < grid.size(); x++) {
			cout << across[x] << '\t';
			for(size_t y = 0; y < grid[x].size(); y++) {
				cout << grid[x][y] << '\t';
			}
			cout << endl << endl << endl;
		}
	}

	return 0;
}
