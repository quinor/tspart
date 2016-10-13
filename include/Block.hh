#pragma once

#include <vector>
#include <algorithm>



namespace block_impl
{
  using timestamp_t = size_t;
  timestamp_t get_timestamp ();


  class Block;
  template <typename Content>
  class DataPromise;
  template <typename Content>
  class DataInput;


  template <typename Content>
  class DataPromise
  {
    friend class DataInput<Content>;
    friend class Block;

  public:

    DataPromise(Block* m);

    const Content& get_data();

  private:

    void update(timestamp_t now = get_timestamp());

    timestamp_t timestamp();

    Content data;
    Block* creator;
  };


  class DataInputMeta
  {
    friend class Block;

  protected:

    virtual void update(timestamp_t) = 0;

    virtual timestamp_t timestamp() = 0;
  };

  class DataInputDummy : public DataInputMeta
  {
  public:

    DataInputDummy(Block* m);

    void force_update();

  protected:

    virtual void update(timestamp_t now = get_timestamp()) override;

    virtual timestamp_t timestamp() override;

  private:
    
    timestamp_t force_update_timestamp;
  };


  template <typename Content>
  class DataInput : public DataInputMeta
  {

  public:

    DataInput(Block* m);

    void connect(DataPromise<Content>& p);

    const Content& get_data();

  protected:

    virtual void update(timestamp_t now = get_timestamp()) override;

    virtual timestamp_t timestamp() override;

  private:

    DataPromise<Content>* provider;
    timestamp_t connect_timestamp;
  };


  class Block
  {
    template <typename Content>
    friend class DataInput;
    template <typename Content>
    friend class DataPromise;
    friend class DataInputDummy;

  public:

    Block();

    void update(timestamp_t now = get_timestamp());
    void refresh();

  protected:

    virtual void compute() = 0; //computes outputs based on inputs

    template <typename Content>
    Content& data_hook(DataPromise<Content>& output);

  private:

    timestamp_t timestamp();


    std::vector<DataInputMeta*> inputs;
    timestamp_t compute_timestamp;
    timestamp_t query_timestamp;
    DataInputDummy dummy_in;
  };



  template <typename Content>
  DataPromise<Content>::DataPromise(Block* m)
  : creator(m)
  {}

  template <typename Content>
  const Content& DataPromise<Content>::get_data()
  {
    return data;
  }

  template <typename Content>
  void DataPromise<Content>::update(timestamp_t now)
  {
    creator->update(now);
  }

  template <typename Content>
  timestamp_t DataPromise<Content>::timestamp()
  {
    return creator->timestamp();
  }


  template <typename Content>
  DataInput<Content>::DataInput(Block* m)
  : provider(nullptr)
  , connect_timestamp(0)
  {
    m->inputs.push_back(this);    
  }

  template <typename Content>
  void DataInput<Content>::connect(DataPromise<Content>& p)
  {
    provider = &p;
    connect_timestamp = get_timestamp();
  }

  template <typename Content>
  const Content& DataInput<Content>::get_data()
  {
    if (provider == nullptr)
      throw "No provider for this input!";
    return provider->data;
  }

  template <typename Content>
  void DataInput<Content>::update(timestamp_t now)
  {
    if (provider == nullptr)
      throw "No provider for this input!";
    provider->update(now);
  }

  template <typename Content>
  timestamp_t DataInput<Content>::timestamp()
  {
    return std::max(connect_timestamp, provider->timestamp());
  }


  template<typename Content>
  Content& Block::data_hook(DataPromise<Content>& output)
  {
    if (output.creator != this)
      throw "No easy hacking, buddy!";
    return output.data;
  }

}

using Block = block_impl::Block;

template<typename Content>
using DataPromise = block_impl::DataPromise<Content>;

template<typename Content>
using DataInput = block_impl::DataInput<Content>;
