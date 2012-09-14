/*
 * Copyright (C) 2008, British Broadcasting Corporation
 * All Rights Reserved.
 *
 * Author: Philip de Nier
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *
 *     * Redistributions of source code must retain the above copyright notice,
 *       this list of conditions and the following disclaimer.
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in the
 *       documentation and/or other materials provided with the distribution.
 *     * Neither the name of the British Broadcasting Corporation nor the names
 *       of its contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
 * LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 * POSSIBILITY OF SUCH DAMAGE.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <memory>

#include <libMXF++/MXF.h>
#include <metadata/EBUCoreDMS++.h>


using namespace std;
using namespace mxfpp;
using namespace EBUCore::KLV;


const mxfKey ebucoreRatingBase::setKey = MXF_SET_K(ebucoreRating);


ebucoreRatingBase::ebucoreRatingBase(HeaderMetadata *headerMetadata)
: InterchangeObject(headerMetadata, headerMetadata->createCSet(&setKey))
{
    headerMetadata->add(this);
}

ebucoreRatingBase::ebucoreRatingBase(HeaderMetadata *headerMetadata, ::MXFMetadataSet *cMetadataSet)
: InterchangeObject(headerMetadata, cMetadataSet)
{}

ebucoreRatingBase::~ebucoreRatingBase()
{}


std::string ebucoreRatingBase::getratingValue() const
{
    return getStringItem(&MXF_ITEM_K(ebucoreRating, ratingValue));
}

std::string ebucoreRatingBase::getratingScaleMinValue() const
{
    return getStringItem(&MXF_ITEM_K(ebucoreRating, ratingScaleMinValue));
}

std::string ebucoreRatingBase::getratingScaleMaxValue() const
{
    return getStringItem(&MXF_ITEM_K(ebucoreRating, ratingScaleMaxValue));
}

std::string ebucoreRatingBase::getratingLanguage() const
{
    return getStringItem(&MXF_ITEM_K(ebucoreRating, ratingLanguage));
}

ebucoreTypeGroup* ebucoreRatingBase::getratingTypeGroup() const
{
    auto_ptr<MetadataSet> obj(getStrongRefItem(&MXF_ITEM_K(ebucoreRating, ratingTypeGroup)));
    MXFPP_CHECK(dynamic_cast<ebucoreTypeGroup*>(obj.get()) != 0);
    return dynamic_cast<ebucoreTypeGroup*>(obj.release());
}

ebucoreFormatGroup* ebucoreRatingBase::getratingFormatGroup() const
{
    auto_ptr<MetadataSet> obj(getStrongRefItem(&MXF_ITEM_K(ebucoreRating, ratingFormatGroup)));
    MXFPP_CHECK(dynamic_cast<ebucoreFormatGroup*>(obj.get()) != 0);
    return dynamic_cast<ebucoreFormatGroup*>(obj.release());
}

ebucoreEntity* ebucoreRatingBase::getratingProviderEntity() const
{
    auto_ptr<MetadataSet> obj(getStrongRefItem(&MXF_ITEM_K(ebucoreRating, ratingProviderEntity)));
    MXFPP_CHECK(dynamic_cast<ebucoreEntity*>(obj.get()) != 0);
    return dynamic_cast<ebucoreEntity*>(obj.release());
}

void ebucoreRatingBase::setratingValue(std::string value)
{
    setStringItem(&MXF_ITEM_K(ebucoreRating, ratingValue), value);
}

void ebucoreRatingBase::setratingScaleMinValue(std::string value)
{
    setStringItem(&MXF_ITEM_K(ebucoreRating, ratingScaleMinValue), value);
}

void ebucoreRatingBase::setratingScaleMaxValue(std::string value)
{
    setStringItem(&MXF_ITEM_K(ebucoreRating, ratingScaleMaxValue), value);
}

void ebucoreRatingBase::setratingLanguage(std::string value)
{
    setStringItem(&MXF_ITEM_K(ebucoreRating, ratingLanguage), value);
}

void ebucoreRatingBase::setratingTypeGroup(ebucoreTypeGroup* value)
{
    setStrongRefItem(&MXF_ITEM_K(ebucoreRating, ratingTypeGroup), value);
}

void ebucoreRatingBase::setratingFormatGroup(ebucoreFormatGroup* value)
{
    setStrongRefItem(&MXF_ITEM_K(ebucoreRating, ratingFormatGroup), value);
}

void ebucoreRatingBase::setratingProviderEntity(ebucoreEntity* value)
{
    setStrongRefItem(&MXF_ITEM_K(ebucoreRating, ratingProviderEntity), value);
}
