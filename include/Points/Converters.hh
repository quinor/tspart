#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"


class ImageToScalarField : public Block,
  public AsOutput<ScalarField<uint8_t>, ImageToScalarField>
{
public:

  ImageToScalarField();

protected:

  virtual void compute() override;

public:

  DataInput<sf::Texture> in;
  DataPromise<ScalarField<uint8_t>> out;

  DataInput<size_t> scale_input;
  Input<size_t> scale_manual;
};


class ScalarFieldMassPrefixSum : public Block,
  public AsOutput<ScalarField<MassElement>, ScalarFieldMassPrefixSum>
{
public:

  ScalarFieldMassPrefixSum();

protected:

  virtual void compute() override;

public:

  DataInput<ScalarField<uint8_t>> in;
  DataPromise<ScalarField<MassElement>> out;
};
