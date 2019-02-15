#pragma once

#include "utils/Logger.hh"
#include "utils/Block.hh"
#include "utils/UtilityBlocks.hh"
#include "DataTypes.hh"


class GrayscaleImageToScalarField : public Block,
  public AsOutput<ScalarField<uint8_t>, GrayscaleImageToScalarField>
{
public:

  GrayscaleImageToScalarField();

protected:

  virtual void compute() override;

public:

  DataInput<sf::Texture> in;
  DataPromise<ScalarField<uint8_t>> out;

  DataInput<size_t> scale_input;
  Input<size_t> scale_manual;
};


class ColorImageToScalarField : public Block,
  public AsOutput<ScalarField<sf::Color>, ColorImageToScalarField>
{
public:

  ColorImageToScalarField();

protected:

  virtual void compute() override;

public:

  DataInput<sf::Texture> in;
  DataPromise<ScalarField<sf::Color>> out;

  DataInput<size_t> scale_input;
  Input<size_t> scale_manual;
};


class ScalarFieldMassPrefixSum : public Block,
  public AsOutput<ScalarField<WeightedElement>, ScalarFieldMassPrefixSum>
{
public:

  ScalarFieldMassPrefixSum();

protected:

  virtual void compute() override;

public:

  DataInput<ScalarField<uint8_t>> in;
  DataPromise<ScalarField<WeightedElement>> out;
};


class ScalarFieldColorPrefixSum : public Block,
  public AsOutput<ScalarField<WeightedElement>, ScalarFieldColorPrefixSum>
{
public:

  ScalarFieldColorPrefixSum();

protected:

  virtual void compute() override;

public:

  DataInput<ScalarField<sf::Color>> in;
  DataPromise<ScalarField<WeightedElement>> out;
};
