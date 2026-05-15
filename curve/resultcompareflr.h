#pragma once

class ResultCompareFLR {
public:
	ResultCompareFLR(int paramsTotal, int dimsFailsTotal, double dimsFailsDevitationSum, int pointsFailsTotal,
		double pointsFailsDevitationSum, double maxDeviationFail);
	~ResultCompareFLR();

	int paramsTotal();
	int failsTotal();
	int dimsFailsTotal();
	int pointsFailsTotal();
	double dimsFailsDevitationSum();
	double pointsFailsDevitationSum();
	double getAverageDimDevFail();
	double getAveragePointDevFail();
	double maxDeviationFail();

private:
	int _paramsTotal;
	int _failsTotal;
	int _dimsFailsTotal;
	int _pointsFailsTotal;
	double _dimsFailsDevitationSum;
	double _pointsFailsDevitationSum;
	double _maxDeviationFail;
};

class ResultCompare2Params {
public:
	ResultCompare2Params(QString name1, QString name2, double value1, double value2, double upperTolerance, double lowerTolerance);
	bool isDiffBiggerThanPrecision(double precision);
	bool isDiffBiggerThanTolerance(double tolerancePercent);
	QString toQString(double precision);

	QString name1();
	QString name2();
	double value1();
	double value2();
	double diff();
	double ut();
	double lt();
	double error();
	bool isComparable();

private:
	QString _name1;
	QString _name2;
	double _value1;
	double _value2;
	double _upperTolerance;
	double _lowerTolerance;
	double _diff;
	double _error;
	bool _isComparable;
};
