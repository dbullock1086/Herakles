Double_t PulseShape(Double_t *x, Double_t *par)
{
  /* par[0] is pedestal
   * par[1] is height
   * par[2] is phase
   * par[3] is width
   */

  /* the function can only accept:
   * ... positive pedestal values
   * ... positive phase values
   * ... and positive width values
   * but it CAN accept negative height (negative pulses)
   */
  if (par[0]<0) return 0;
  if (par[2]<0) return 0;
  if (par[3]<0) return 0;

  const Double_t pulse[100] = {
    -0.00066,  0.00194,  0.00005, -0.00236, -0.00170,  0.00044,  0.00133,
     0.00088,  0.00019, -0.00012, -0.00018, -0.00037, -0.00073, -0.00063,
     0.00148,  0.00733,  0.01792,  0.03352,  0.05468,  0.08233,  0.11697,
     0.15842,  0.20641,  0.26093,  0.32185,  0.38857,  0.46007,  0.53488,
     0.61101,  0.68571,  0.75555,  0.81753,  0.87042,  0.91497,  0.95234,
     0.98038,  0.99559,  1.00000,  0.99470,  0.97875,  0.95271,  0.91860,
     0.87812,  0.83228,  0.78239,  0.73022,  0.67699,  0.62298,  0.56822,
     0.51314,  0.45855,  0.40556,  0.35548,  0.30968,  0.26919,  0.23406,
     0.20313,  0.17517,  0.15038,  0.12972,  0.11287,  0.09823,  0.08490,
     0.07340,  0.06419,  0.05690,  0.05077,  0.04532,  0.04035,  0.03576,
     0.03148,  0.02745,  0.02361,  0.01991,  0.01634,  0.01291,  0.00965,
     0.00658,  0.00375,  0.00118, -0.00110, -0.00305, -0.00465, -0.00585,
    -0.00664, -0.00699, -0.00693, -0.00651, -0.00581, -0.00493, -0.00398,
    -0.00304, -0.00215, -0.00136, -0.00068, -0.00008,  0.00042,  0.00086,
     0.00124,  0.00158
  };

  // what is the max/min x-value that will fit this distribution
  // for the given parameters?
  // FWHM is 11.356 ...
  // pulse peak occurs at 37% of the array size
  const Double_t xMin = par[2] - 3.258*par[3];
  // array size is 8.806 times the FWHM
  const Double_t xMax = xMin + 8.806*par[3];
  // outside of this shape's range, return a flat pedestal
  if (x[0]<xMin || x[0]>xMax) return par[0];

  // the x-values should be scaled up to the size of the array
  const Double_t xScale = 11.356 / par[3];
  // value should shift left by...
  const Double_t xOff = 0.8806*par[3];

  // calculate where the current value should fit in the distribution
  const Double_t xVal = (x[0]-xMin) * xScale;
  // the array can only accept integer values
  const Int_t xInt = static_cast<int>(xVal);

  // quick return in case the x-value is already an integer
  if (xVal-xInt == 0) return par[0] + par[1]*pulse[xInt];

  // pedestal + height * yvalue (from the bin's lower edge)
  const Double_t arrValLow = par[0] + par[1]*pulse[xInt];
  // pedestal + height * yvalue (from the next bin in the array)
  Double_t arrValHigh = par[0]; //  pedestal or overflow
  if (xInt<99) arrValHigh += par[1]*pulse[xInt+1];

  // slope of linear interpolation
  const Double_t slope = arrValHigh - arrValLow;
  // intercept of linear interpolation
  const Double_t intercept = arrValLow - slope*xInt;

  // return the interpolated pulse height from the actual value
  return slope*xVal + intercept;
}
