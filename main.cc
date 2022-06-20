#include <iostream>
#include <vector>
#include <utility>
#include <algorithm>
#include <string>
using namespace std;

typedef vector<int> vi;
typedef vector<vector<int>> vvi;
typedef vector<double> vd;
typedef vector<vector<double>> vvd;

pair<vi,double> viterbiPath(vvd V, vvi backPointers, int observations, int states)
{
  double maxProb = V[0][observations-1];
  int maxProbIdx = 0; 

  for(int i = 1 ; i < states; i++)
  {
    if(V[i][observations-1] > maxProb)
    {
      maxProb = V[i][observations-1];
      maxProbIdx = i;
    }
  }
  vi path(observations);
  path[observations-1] = maxProbIdx;

  for(int i = observations-2; i > 0; i--)
    path[i] = backPointers[path[i+1]][i+1];

  return make_pair(path, maxProb);
}

void viterbi(vvd& V, vvi& backPointers, int observations, int states, vd start, vvd trans, vvd emit)
{
  // 
  for (int i = 0; i < states; i++)
  {
    V[i][0] = start[i] * emit[i][0];
    backPointers[i][0] = -1;
  }
  
  for (int i = 1; i < observations; i++)
  {
    for (int j = 0; j < states; j++)
    {

      double maxProb = V[0][i - 1] * trans[0][j];
      int prevStSelected = 0;

      for (int k = 1; k < states; k++)
      {
        double currProb = V[k][i - 1] * trans[k][j];
        if (currProb > maxProb)
        {
          maxProb = currProb;
          prevStSelected = k;
        }
      }
      maxProb = maxProb * emit[j][i];
      V[j][i] = maxProb;
      backPointers[j][i] = prevStSelected;
    }
  }
}

int main()
{
  vector<string> statesS = {"DT", "NN", "VB"}; 
  vector<string> observationsS = {"THE", "FANS", "WATCH", "THE", "RACE"};

  size_t states, observations;
  vvd emmissions, transitions, V;
  vd start;
  vvi backPointers;
  pair<vi,double> answer;

  states = 3, observations = 5;

  
  V.resize(states, vd(observations));
  backPointers.resize(states, vi(observations));

  start = {0.8, 0.2, 0};

  transitions = {
      {0, 0.9, 0.1},
      {0, 0.5, 0.5},
      {0.5, 0.5, 0,}
  };

  emmissions = {
      {0.2, 0, 0, 0.2, 0},
      {0, 0.1, 0.3, 0, 0.1},
      {0, 0.2, 0.15, 0, 0.3}};

  viterbi(V,backPointers,observations,states,start,transitions,emmissions);

  answer = viterbiPath(V, backPointers,observations,states);

  cout<<"Viterbi Path"<<'\n';
  for(int i : answer.first)
    cout<<i<<" ";
  cout<<'\n';

  cout<<"Viterbi Path (states - observations)"<<'\n';
  for(int i : answer.first)
    cout<<statesS[i]<<" ";
  cout<<'\n';

  for(string i : observationsS)
    cout<<i<<" ";
  cout<<'\n';
  cout<<"Max Prob: "<<answer.second;

  return 0;
}