##############################################################################
#
# Library:   TubeTK
#
# Copyright 2010 Kitware Inc. 28 Corporate Drive,
# Clifton Park, NY, 12065, USA.
#
# All rights reserved.
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
##############################################################################

set( FLTK_URL http://midas3.kitware.com/midas/download/item/208771/fltk-1.3.2.20130812084657.source.tar.gz )
set( FLTK_URL_MD5_TAG f8b2e249701cf77520546e3c37130f0b )

# Insight Segmentation and Registration Toolkit
set( ITK_URL ${github_protocol}://github.com/Slicer/ITK.git )
set( ITK_URL_OR_TAG 56fae278ad0ae805da4f4dbea5a4b9979cf4262c )

set( CTK_URL https://github.com/commontk/CTK.git )
set( CTK_URL_MD5_TAG 1c97e5426f898bc7d074e6122992d0dd12bab56b )

set( VTK_URL https://github.com/Slicer/VTK.git )
set( VTK_URL_MD5_TAG b55dad7dbdf8bdbe25d814b411aa5f211b68c09f )

# JsonCpp snapshot 2014-04-15 r276
set( JsonCpp_URL http://midas3.kitware.com/midas/download/bitstream/366544/JsonCpp_r276.tar.gz )
set( JsonCpp_HASH_OR_TAG 192f0cf2b00798d4f4fb29c99a3aa83c )

# TubeTK Parameter Serializer snapshot 2014-02-05 )
set( ParameterSerializer_URL
  https://github.com/Slicer/ParameterSerializer.git )
set( ParameterSerializer_HASH_OR_TAG d7e94d3ff9de9a8782db6997f8e1bc45626cfa3b )

# Slicer Execution Model snapshot 2014-02-18
set( SlicerExecutionModel_URL
  ${github_protocol}://github.com/Slicer/SlicerExecutionModel.git )
set( SlicerExecutionModel_HASH_OR_TAG a6616b527dc0dced26256f6a303174a98f373f6f )
