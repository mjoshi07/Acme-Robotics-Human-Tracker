/******************************************************************************
 * MIT License
 * 
 * Copyright (c) 2021 Mayank Joshi, Naitri Rajyaguru
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE. 
 ******************************************************************************/

/**
 * @file HumanTracker.cpp
 * @author Mayank Joshi (driver) and Naitri Rajyaguru (navigator)
 * @brief HumanTracker class definition for Acme Robotics - Human Tracker
 * @version 0.1
 * @date 2021-10-05
 * 
 * @copyright Copyright (c) 2021
 * 
 */


#include <HumanTracker.hpp>


acme::HumanTracker::HumanTracker(double confidence) {
    InitParams(confidence);
}

acme::HumanTracker::~HumanTracker() {}

void acme::HumanTracker::InitParams(double confidence) {
    conf_thresh_ = confidence;
    double detector_conf(0.2);
    std::vector<std::string> classes{"person"};
    detector_ = std::make_unique<acme::Detector>(detector_conf, classes);
    humans_.clear();
    tracker_ = cv::TrackerKCF::create();
}

std::vector<cv::Rect> acme::HumanTracker::TrackHumans(const cv::Mat &frame) {
    humans_.clear();
    std::vector<acme::Detection> output = detector_->Detect(frame);
    RemoveNoise(output);
    return humans_;
}

void acme::HumanTracker::RemoveNoise(const std::vector<acme::Detection> &d) {
    for ( acme::Detection detect : d ) {
        if (detect.confidence >= conf_thresh_) {
            humans_.push_back(detect.bbox);
        }
    }
}
