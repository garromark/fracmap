/*
 *  LinRegression.h
 *  wxFracMap
 *
 *  Created by Shammah Chancellor and Mark Garro on 18/05/2009.
 *  Copyright 2009 University Of Neveda, Reno. All rights reserved.
 *
 */

template<class X, class Y>
class LinRegression
  {
  public:
	LinRegression() {set = false;}
	~LinRegression() {}
	
	double slope()
	{
	  if(!set) regress();
	  
	  return Sxy / Sxx;
	}
	double Rsquared()
	{
	  if(!set) regress();
	  
	  return Sxy / sqrt(Sxx * Syy);
	}
	double intercept()
	{
	  if(!set) regress();
	  
	  return ybar - slope() * xbar;
	}
	
	void regress()
	{
	  // Malformed data.
	  if(x.size() != y.size())
	  {
		Sxy = 0.0;
		Sxx = 1.0;
		Syy = 1.0;
		
		return;
	  }
	  
	  set = true;
	  Sxx = 0.0;
	  Syy = 0.0;
	  Sxy = 0.0;
	  xbar = 0;
	  ybar = 0;
	  
	  for(unsigned int i = 0; i < x.size(); i++)
	  {
		xbar += x[i];
		ybar += y[i];
	  }
	  
	  xbar /= x.size();
	  ybar /= y.size();
	  
	  for(unsigned int i = 0; i < x.size(); i++)
	  {
		Sxx += pow((x[i] - xbar), 2.0);
		Syy += pow((y[i] - ybar), 2.0);
		Sxy += (x[i] - xbar) * (y[i] - ybar);
	  }
	}
	
	void add_x(X new_x) { x.push_back(new_x); }
	void add_y(Y new_y) { y.push_back(new_y); }
	
	bool set;
	
  private:
	vector<X> x;
	vector<Y> y;
	double Sxx, Syy, Sxy;
	double xbar, ybar;
	
  };
