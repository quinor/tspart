#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "DataTypes.hh"


class ImageToScalarFieldConverter : public Block
{
public:

  ImageToScalarFieldConverter();

private:

  virtual void compute() override;


public:

  DataInput<sf::Texture> in;
  DataPromise<ScalarField> out;

  DataInput<size_t> scale_input;
  DataPromiseManual<size_t> scale_manual;
};
