#include "ImageWriterFitsTest.h"
#include "modules/ImageWriterFits.h"
#include "utility/Config.h"
#include "data/ImageData.h"
#include <QCoreApplication>
#include <QFile>
#include "utility/pelicanTimer.h"
#include "data/DataBlob.h"
#include <iostream>

#include "utility/memCheck.h"

namespace pelican {

CPPUNIT_TEST_SUITE_REGISTRATION( ImageWriterFitsTest );

// class ImageWriterFitsTest
ImageWriterFitsTest::ImageWriterFitsTest()
    : CppUnit::TestFixture()
{
}

ImageWriterFitsTest::~ImageWriterFitsTest()
{
}

void ImageWriterFitsTest::setUp()
{
    int ac = 0;
    _app = new QCoreApplication(ac, NULL);
}

void ImageWriterFitsTest::tearDown()
{
    delete _app;
}

void ImageWriterFitsTest::test_createModule()
{
    QDomElement config;
    ImageWriterFits fits(config);
}


void ImageWriterFitsTest::test_openFile()
{
    QDomElement config;
    ImageWriterFits fits(config);

    ImageData image(128, 128, 512, 2);
    fits._image = &image;
    fits._open("testEmpty", 128, 128, 1, 1, true);
    fits._writeHeader();
    real_t* im = image.ptr();
    fits._writeImage(im, 128, 128, 0, 0);
    fits._close();
}


void ImageWriterFitsTest::test_createImage()
{
    QDomElement config;
    ImageWriterFits fits(config);
    unsigned nL = 128;
    unsigned nM = 128;
    unsigned nChan = 1;
    unsigned nPol = 1;
    ImageData image(nL, nM, nChan, nPol);
    real_t* amp = image.ptr();
    for (unsigned m = 0; m < image.sizeM(); m++) {
        for (unsigned l = 0; l < image.sizeL(); l++) {
            unsigned index = m * image.sizeL() + l;
            amp[index] = static_cast<real_t>(m);
        }
    }
    fits._image = &image;
    TIMER_START
    fits._open("test", nL, nM, nChan, nPol, true);
    fits._writeHeader();
    real_t* im = image.ptr();
    fits._writeImage(im, nL, nL, 0, 0);
    TIMER_STOP("ImageWriterFitsTest::test_createImage()");
    fits._close();
}


/**
 * @details
 */
void ImageWriterFitsTest::test_createCube()
{
    QDomElement config;
    ImageWriterFits fits(config);
    unsigned nL = 128;
    unsigned nM = 128;
    unsigned nChan = 2;
    unsigned nPol = 2;
    ImageData image(nL, nM, nChan, nPol);
    real_t* amp = image.ptr();
    unsigned nPixelsPerPol = nM * nL;
    unsigned nPixelsPerChan = nM * nL * nPol;
    unsigned nPixelsPerRow = nL;

    for (unsigned c = 0; c < nChan; c++) {
        for (unsigned p = 0; p < nPol; p++) {
            for (unsigned m = 0; m < image.sizeM(); m++) {
                for (unsigned l = 0; l < image.sizeL(); l++) {
                    unsigned index = c * nPixelsPerChan + p * nPixelsPerPol + m * nPixelsPerRow + l;
                    if (c == 0) {
                        if (p == 0) amp[index] = static_cast<real_t>(m);
                        if (p == 1) amp[index] = static_cast<real_t>(l);
                    }
                    if (c == 1) {
                        if (p == 0) amp[index] = static_cast<real_t>(l) + static_cast<real_t>(m);
                        if (p == 1) amp[index] = static_cast<real_t>(l) - static_cast<real_t>(m);
                    }
                }
            }
        }
    }
    fits._image = &image;
    TIMER_START
    fits._open("testCube", nL, nM, nChan, nPol, true);
    fits._writeHeader();
    fits._writeImage(image.ptr(0, 0), nL, nL, 0, 0);
    fits._writeImage(image.ptr(0, 1), nL, nL, 0, 1);
    fits._writeImage(image.ptr(1, 0), nL, nL, 1, 0);
    fits._writeImage(image.ptr(1, 1), nL, nL, 1, 1);
    TIMER_STOP("ImageWriterFitsTest::test_createCube()");
    fits._close();
}


} // namespace pelican
