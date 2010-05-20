///
/// \class boost::numeric::ublas::c_vector
/// \brief An array based vector class which size is defined at type specification or object instanciation.
/// An array based matrix class which size is defined at type specification or object instanciation. This matrix is directly based on a predefinec C-style arry of data, thus providing the fastest implementation possible. The constraint is that dimensions of the matrix must be specified at the instanciation or the type specification. For instance, \code typedef c_matrix<double,4,4> my_4by4_matrix\encode defines a 4x4 double-precision matrix. You can also instantiate it directly with \code c_matrix<int,8,5> my_fast_matrix\endcode. This will make a 8 by 5 integer matrix. The price to pay for this speed is that you cannot resize it to a size larger than the one defined in the template parameters. In the previous example, a size of 4 by 5 or 3 by 2 is acceptable, but a new size of 9 by 5 or even 10 by 10 will raise a bad_size() exception.
///
