#include <iostream>
#include <vector>
#include <fstream>
#include <cmath>
#include <random>
#include <boost/math/distributions/beta.hpp>
#include <boost/math/distributions/inverse_gamma.hpp>
#include "Metodos.h"

std::mt19937 generator(clock());
std::uniform_real_distribution<double> distribution(0.0,1.0);

double beta_density(double x, double alpha, double beta)
{
  double beta_func = (std::tgamma(alpha) * std::tgamma(beta))/std::tgamma(alpha + beta);
  return (std::pow(x, alpha - 1) * std::pow((1-x), beta - 1))/beta_func;
}

double bernoulli_verossimi(std::vector<double> x, double p)
{
  double sum = 0;
  for (int i = 0; i < x.size(); i++)
  {
    sum += x[i];
  }

  return (pow(p, sum) * pow(1-p, (x.size()-sum)));
}

void SIR(std::vector<double> x, double alpha, double beta, double J)
{
  std::vector<double> samples;
  samples.resize(J);

  boost::math::beta_distribution<> mybeta(alpha, beta);
  double randFromUnif = distribution(generator);

  for (int i = 0; i < J; i++)
  {
    samples[i] = quantile(mybeta, randFromUnif);
    randFromUnif = distribution(generator);
  }

  std::vector<double> w;
  w.resize(J);

  for (int i = 0; i < J; i++)
  {
    w[i] = bernoulli_verossimi(x, samples[i]);
  }

  std::discrete_distribution<int> sampling (w.begin(), w.end());

  std::vector<double> final_sample;
  int idx;
  final_sample.resize(J/20) ;

  for (int i = 0; i < J/20; i++)
  {
    idx = sampling(generator);
    final_sample[i] = samples[idx];
  }

  std::ofstream newfile;
  newfile.open("SIR.csv");

  for (int i = 0; i < final_sample.size(); i++)
  {
    newfile << final_sample[i];
    newfile << ",";
  }

  newfile.close();
}

void Rejection(std::vector<double> x, double alpha, double beta, double J)
{
  std::vector<double> final_sample;
  final_sample.resize(J);

  boost::math::beta_distribution<> mybeta(alpha, beta);

  //O estimador de max verossimilhança de uma bernoulli e sua media amostral
  double sum = 0;
  for (int i = 0; i < x.size(); i++)
  {
    sum+= x[i];
  }

  double M = pow(sum/x.size(),sum) * pow (1 - sum/x.size(), x.size() - sum);

  double cont = 0;
  double randFromUnif = distribution(generator), candidate;
  while (cont < J)
  {
    candidate = quantile(mybeta, randFromUnif);
    randFromUnif = distribution(generator);

    //Considerando a g(O) como a priori temos que a parte direita da ineq sera a verossimilhança sobre o Estimador de MaxVer
    if (randFromUnif <= bernoulli_verossimi(x, candidate)/M)
    {
      final_sample[cont] = candidate;
      cont++;
    }
  }

  std::ofstream newfile;
  newfile.open("Rejection.csv");

  for (int i = 0; i < final_sample.size(); i++)
  {
    newfile << final_sample[i];
    newfile << ",";
  }

  newfile.close();
}

void Metropolis_Hastings(std::vector<double> x, double alpha, double beta, double T)
{
  std::vector<double> prev_teta;
  double act_teta;

  prev_teta.resize(T+1);
  prev_teta[0] = 0.5;

  // std::uniform_real_distribution<double> distribution(0.0,1.0);
  boost::math::beta_distribution<> mybeta(alpha, beta);

  double randFromUnif, R;
  for (int i = 0; i < T; i++)
  {
    randFromUnif = distribution(generator);
    act_teta = quantile(mybeta, randFromUnif);

    //Calculando a razao para o caso bayesiano com g() igual a priori
    //IMPORTANTE verificar ordem da razao, slide professora ta diferente do Pedro
    R = bernoulli_verossimi(x, act_teta)/bernoulli_verossimi(x, prev_teta[i]);
    if (R > 1)
    {
      R = 1;
    }
    randFromUnif = distribution(generator);
    if (R >= randFromUnif)
    {
      prev_teta[i+1] = act_teta;
    }
    else
    {
      prev_teta[i+1] = prev_teta[i];
    }
  }

  std::ofstream newfile;
  newfile.open("Metropolis_Hastings.csv");

  for (int i = 0; i < prev_teta.size(); i++)
  {
    newfile << prev_teta[i];
    newfile << ",";
  }

  newfile.close();
}

void Gibbs_Sampling(std::vector<double> x, double m, double V, double a, double b, double T)
{
  // boost::math::inverse_gamma_distribution<> mygamma;
  // std::normal_distribution<double> normal_dist;
  double randFromUnif;

  std::vector<double> u, sigma;
  u.resize(T);
  sigma.resize(T + 1);
  sigma[0] = 1;
  double sumsqr, n = x.size(), x_mean = 0;

  for (int i = 0; i < x.size(); i++)
  {
    x_mean += x[i];
  }
  x_mean = x_mean/x.size();

  for (int i = 0; i < T; i++)
  {
    std::normal_distribution<double> normal_dist((m + n*V*x_mean)/(n*V + 1), (sigma[i] * V)/(n*V + 1));
    u[i] = normal_dist(generator);
    sumsqr = 0;
    for (int j = 0; j < x.size(); j++)
    {
      sumsqr += pow(x[j] - u[i], 2);
    }
    boost::math::inverse_gamma_distribution<> mygamma(a + (n + 1)/2, b + pow(u[i] - m,2)/2*V + sumsqr/2);
    randFromUnif = distribution(generator);
    sigma[i + 1] = quantile(mygamma, randFromUnif);
  }

  std::ofstream newfile;
  newfile.open("Gibbs_Sampling.csv");

  newfile << u[0];
  for (int i = 1; i < u.size(); i++)
  {
    newfile << ",";
    newfile << u[i];
  }
  newfile << "\n";
  newfile << sigma[0];
  for (int i = 1; i < sigma.size();i++)
  {
    newfile << ",";
    newfile << sigma[i];
  }
  newfile.close();
}
