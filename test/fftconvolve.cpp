/*******************************************************
 * Copyright (c) 2014, ArrayFire
 * All rights reserved.
 *
 * This file is distributed under 3-clause BSD license.
 * The complete license agreement can be obtained at:
 * http://arrayfire.com/licenses/BSD-3-Clause
 ********************************************************/

#include <gtest/gtest.h>
#include <arrayfire.h>
#include <af/dim4.hpp>
#include <af/traits.hpp>
#include <string>
#include <vector>
#include <testHelpers.hpp>

using std::vector;
using std::string;

template<typename T>
class FFTConvolve : public ::testing::Test
{
    public:
        virtual void SetUp() {}
};

// create a list of types to be tested
typedef ::testing::Types<float, double, int, uint, char, uchar> TestTypes;

// register the type list
TYPED_TEST_CASE(FFTConvolve, TestTypes);

template<typename T, int baseDim>
void fftconvolveTest(string pTestFile, bool expand)
{
    if (noDoubleTests<T>()) return;

    using af::dim4;

    vector<dim4>      numDims;
    vector<vector<T> >      in;
    vector<vector<T> >   tests;

    readTests<T, T, int>(pTestFile, numDims, in, tests);

    dim4 sDims        = numDims[0];
    dim4 fDims        = numDims[1];
    af_array signal   = 0;
    af_array filter   = 0;
    af_array outArray = 0;

    ASSERT_EQ(AF_SUCCESS, af_create_array(&signal, &(in[0].front()),
                sDims.ndims(), sDims.get(), (af_dtype)af::dtype_traits<T>::af_type));
    ASSERT_EQ(AF_SUCCESS, af_create_array(&filter, &(in[1].front()),
                fDims.ndims(), fDims.get(), (af_dtype)af::dtype_traits<T>::af_type));

    switch(baseDim) {
        case 1: ASSERT_EQ(AF_SUCCESS, af_fftconvolve1(&outArray, signal, filter, expand)); break;
        case 2: ASSERT_EQ(AF_SUCCESS, af_fftconvolve2(&outArray, signal, filter, expand)); break;
        case 3: ASSERT_EQ(AF_SUCCESS, af_fftconvolve3(&outArray, signal, filter, expand)); break;
    }

    vector<T> currGoldBar = tests[0];
    size_t nElems         = currGoldBar.size();
    T *outData            = new T[nElems];

    ASSERT_EQ(AF_SUCCESS, af_get_data_ptr((void*)outData, outArray));

    for (size_t elIter=0; elIter<nElems; ++elIter) {
        ASSERT_NEAR(currGoldBar[elIter], outData[elIter], 1e-3)<< "at: " << elIter<< std::endl;
    }

    delete[] outData;
    ASSERT_EQ(AF_SUCCESS, af_destroy_array(outArray));
    ASSERT_EQ(AF_SUCCESS, af_destroy_array(signal));
    ASSERT_EQ(AF_SUCCESS, af_destroy_array(filter));
}

TYPED_TEST(FFTConvolve, Vector)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector.test"), true);
}

TYPED_TEST(FFTConvolve, Rectangle)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle.test"), true);
}

TYPED_TEST(FFTConvolve, Cuboid)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid.test"), true);
}

TYPED_TEST(FFTConvolve, Vector_Many2One)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_many2one.test"), true);
}

TYPED_TEST(FFTConvolve, Rectangle_Many2One)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_many2one.test"), true);
}

TYPED_TEST(FFTConvolve, Cuboid_Many2One)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_many2one.test"), true);
}

TYPED_TEST(FFTConvolve, Vector_Many2Many)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_many2many.test"), true);
}

TYPED_TEST(FFTConvolve, Rectangle_Many2Many)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_many2many.test"), true);
}

TYPED_TEST(FFTConvolve, Cuboid_Many2Many)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_many2many.test"), true);
}

TYPED_TEST(FFTConvolve, Vector_One2Many)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_one2many.test"), true);
}

TYPED_TEST(FFTConvolve, Rectangle_One2Many)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_one2many.test"), true);
}

TYPED_TEST(FFTConvolve, Cuboid_One2Many)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_one2many.test"), true);
}

TYPED_TEST(FFTConvolve, Same_Vector)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_same.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Rectangle)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_same.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Cuboid)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_same.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Vector_Many2One)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_same_many2one.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Rectangle_Many2One)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_same_many2one.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Cuboid_Many2One)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_same_many2one.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Vector_Many2Many)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_same_many2many.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Rectangle_Many2Many)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_same_many2many.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Cuboid_Many2Many)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_same_many2many.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Vector_One2Many)
{
    fftconvolveTest<TypeParam, 1>(string(TEST_DIR"/convolve/vector_same_one2many.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Rectangle_One2Many)
{
    fftconvolveTest<TypeParam, 2>(string(TEST_DIR"/convolve/rectangle_same_one2many.test"), false);
}

TYPED_TEST(FFTConvolve, Same_Cuboid_One2Many)
{
    fftconvolveTest<TypeParam, 3>(string(TEST_DIR"/convolve/cuboid_same_one2many.test"), false);
}

TEST(FFTConvolve1, CPP)
{
    if (noDoubleTests<float>()) return;

    using af::dim4;

    vector<dim4>      numDims;
    vector<vector<float> >      in;
    vector<vector<float> >   tests;

    readTests<float, float, int>(string(TEST_DIR"/convolve/vector.test"), numDims, in, tests);

    //![ex_image_convolve1]
    //vector<dim4> numDims;
    //vector<vector<float> > in;
    af::array signal(numDims[0], &(in[0].front()));
    //signal dims = [32 1 1 1]
    af::array filter(numDims[1], &(in[1].front()));
    //filter dims = [4 1 1 1]

    af::array output = fftconvolve1(signal, filter, true);
    //output dims = [32 1 1 1] - same as input since expand(3rd argument is false)
    //None of the dimensions > 1 has lenght > 1, so no batch mode is activated.
    //![ex_image_convolve1]

    vector<float> currGoldBar = tests[0];
    size_t nElems  = output.elements();
    float *outData = new float[nElems];
    output.host(outData);

    for (size_t elIter=0; elIter<nElems; ++elIter) {
        ASSERT_NEAR(currGoldBar[elIter], outData[elIter], 1e-3)<< "at: " << elIter<< std::endl;
    }

    delete[] outData;
}

TEST(FFTConvolve2, CPP)
{
    if (noDoubleTests<float>()) return;

    using af::dim4;

    vector<dim4>      numDims;
    vector<vector<float> >      in;
    vector<vector<float> >   tests;

    readTests<float, float, int>(string(TEST_DIR"/convolve/rectangle_one2many.test"), numDims, in, tests);

    //![ex_image_convolve2]
    //vector<dim4> numDims;
    //vector<vector<float> > in;
    af::array signal(numDims[0], &(in[0].front()));
    //signal dims = [15 17 1 1]
    af::array filter(numDims[1], &(in[1].front()));
    //filter dims = [5 5 2 1]

    af::array output = fftconvolve2(signal, filter, true);
    //output dims = [15 17 1 1] - same as input since expand(3rd argument is false)
    //however, notice that the 3rd dimension of filter is > 1.
    //So, one to many batch mode will be activated automatically
    //where the 2d input signal is convolved with each 2d filter
    //and the result will written corresponding slice in the output 3d array
    //![ex_image_convolve2]

    vector<float> currGoldBar = tests[0];
    size_t nElems  = output.elements();
    float *outData = new float[nElems];
    output.host(outData);

    for (size_t elIter=0; elIter<nElems; ++elIter) {
        ASSERT_NEAR(currGoldBar[elIter], outData[elIter], 1e-3)<< "at: " << elIter<< std::endl;
    }

    delete[] outData;
}

TEST(FFTConvolve3, CPP)
{
    if (noDoubleTests<float>()) return;

    using af::dim4;

    vector<dim4>      numDims;
    vector<vector<float> >      in;
    vector<vector<float> >   tests;

    readTests<float, float, int>(string(TEST_DIR"/convolve/cuboid_many2many.test"), numDims, in, tests);

    //![ex_image_convolve3]
    //vector<dim4> numDims;
    //vector<vector<float> > in;
    af::array signal(numDims[0], &(in[0].front()));
    //signal dims = [10 11 2 2]
    af::array filter(numDims[1], &(in[1].front()));
    //filter dims = [4 2 3 2]

    af::array output = fftconvolve3(signal, filter, true);
    //output dims = [10 11 2 2] - same as input since expand(3rd argument is false)
    //however, notice that the 4th dimension is > 1 for both signal
    //and the filter, therefore many to many batch mode will be
    //activated where each 3d signal is convolved with the corresponding 3d filter
    //![ex_image_convolve3]

    vector<float> currGoldBar = tests[0];
    size_t nElems  = output.elements();
    float *outData = new float[nElems];
    output.host(outData);

    for (size_t elIter=0; elIter<nElems; ++elIter) {
        ASSERT_NEAR(currGoldBar[elIter], outData[elIter], 1e-3)<< "at: " << elIter<< std::endl;
    }

    delete[] outData;
}

TEST(FFTConvolve, Docs_Unified_Wrapper)
{
    // This unit test doesn't necessarily need to function
    // accuracy as af::convolve is merely a wrapper to
    // af::convolve[1|2|3]
    using af::array;
    using af::dim4;
    using af::randu;
    using af::constant;
    using af::convolve;

    //![ex_image_convolve_1d]
    array a = randu(10);
    //af_print(a);
    //a [10 1 1 1] = 0.0000 0.1315 0.7556 0.4587 0.5328 0.2190 0.0470 0.6789 0.6793 0.9347
    array b = randu(4);
    //af_print(b);
    //b [4 1 1 1]  = 0.3835 0.5194 0.8310 0.0346
    array c = convolve(a, b);
    //af_print(c);
    //c [10 1 1 1] = 0.3581 0.6777 1.0750 0.7679 0.5903 0.4851 0.6598 1.2770 1.0734 0.8002
    //![ex_image_convolve_1d]

    //![ex_image_convolve_2d]
    array d = constant(0.5, 5, 5);
    //af_print(d);
    //d [5 5 1 1]
    //    0.5000     0.5000     0.5000     0.5000     0.5000
    //    0.5000     0.5000     0.5000     0.5000     0.5000
    //    0.5000     0.5000     0.5000     0.5000     0.5000
    //    0.5000     0.5000     0.5000     0.5000     0.5000
    //    0.5000     0.5000     0.5000     0.5000     0.5000
    array e = constant(1, 2, 2);
    //af_print(e);
    //e [2 2 1 1]
    //     1.0000     1.0000
    //     1.0000     1.0000
    array f = fftconvolve(d, e);
    //af_print(f);
    //f [5 5 1 1]
    //     2.0000     2.0000     2.0000     2.0000     1.0000
    //     2.0000     2.0000     2.0000     2.0000     1.0000
    //     2.0000     2.0000     2.0000     2.0000     1.0000
    //     2.0000     2.0000     2.0000     2.0000     1.0000
    //     1.0000     1.0000     1.0000     1.0000     0.5000
    //![ex_image_convolve_2d]

    //![ex_image_convolve_3d]
    array g = constant(1, 4, 4, 4);
    //af_print(g);
    //g [4 4 4 1]
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000

    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000

    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000

    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    //    1.0000     1.0000     1.0000     1.0000
    array h = constant(0.5, 2, 2, 2);
    //af_print(h);
    //h [2 2 2 1]
    //    0.5000     0.5000
    //    0.5000     0.5000

    //    0.5000     0.5000
    //    0.5000     0.5000

    array i = fftconvolve(g, h);
    //af_print(i);
    //i [4 4 4 1]
    //    4.0000     4.0000     4.0000     2.0000
    //    4.0000     4.0000     4.0000     2.0000
    //    4.0000     4.0000     4.0000     2.0000
    //    2.0000     2.0000     2.0000     1.0000

    //    4.0000     4.0000     4.0000     2.0000
    //    4.0000     4.0000     4.0000     2.0000
    //    4.0000     4.0000     4.0000     2.0000
    //    2.0000     2.0000     2.0000     1.0000

    //    4.0000     4.0000     4.0000     2.0000
    //    4.0000     4.0000     4.0000     2.0000
    //    4.0000     4.0000     4.0000     2.0000
    //    2.0000     2.0000     2.0000     1.0000

    //    2.0000     2.0000     2.0000     1.0000
    //    2.0000     2.0000     2.0000     1.0000
    //    2.0000     2.0000     2.0000     1.0000
    //    1.0000     1.0000     1.0000     0.5000
    //![ex_image_convolve_3d]
}
