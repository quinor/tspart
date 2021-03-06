#pragma once

#include <vector>
#include <algorithm>
#include <string>
#include "utils/Logger.hh"


namespace block_impl
{
  class Noncopyable {
  public:
    Noncopyable() = default;
    ~Noncopyable() = default;

  private:
    Noncopyable(const Noncopyable&) = delete;
    Noncopyable& operator=(const Noncopyable&) = delete;
  };


  using timestamp_t = size_t;
  timestamp_t get_timestamp ();


  class Block;
  template <typename Content>
  class DataPromise;
  template <typename Content>
  class DataInput;


  template <typename Content>
  class DataPromiseMeta : private Noncopyable
  {
    friend class DataInput<Content>;

  public:

    virtual const Content& get_data() = 0;

  protected:

    virtual void update(timestamp_t now = get_timestamp()) = 0;

    virtual timestamp_t timestamp() = 0;
  };


  template <typename Content>
  class DataPromise : public DataPromiseMeta<Content>
  {
    friend class Block;

  public:

    explicit DataPromise(Block* m);

    virtual const Content& get_data() override;

  private:

    virtual void update(timestamp_t now = get_timestamp()) override;

    virtual timestamp_t timestamp() override;

    Content data;
    Block* creator;
  };


  class DataInputMeta : private Noncopyable
  {
    friend class Block;

  protected:

    virtual void update(timestamp_t) = 0;

    virtual timestamp_t timestamp() = 0;
  };

  class DataInputDummy : public DataInputMeta
  {
  public:

    explicit DataInputDummy(Block* m);

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

    explicit DataInput(Block* m);

    void connect(DataPromiseMeta<Content>& p);

    const Content& get_data();

  protected:

    virtual void update(timestamp_t now = get_timestamp()) override;

    virtual timestamp_t timestamp() override;

  private:

    DataPromiseMeta<Content>* provider;
    timestamp_t connect_timestamp;
  };


  class Block : private Noncopyable
  {
    template <typename Content>
    friend class DataInput;
    template <typename Content>
    friend class DataPromise;
    friend class DataInputDummy;

  public:

    Block();
    virtual ~Block();

    void update(timestamp_t now = get_timestamp());
    void refresh();
    void set_logger(Logger& new_logger);

  protected:

    virtual void compute() = 0; //computes outputs based on inputs

    template <typename Content>
    Content& data_hook(DataPromise<Content>& output);

  private:

    timestamp_t timestamp();


  public:

    DataPromise<int> done;

  protected:

    std::string name;
    Logger& logger;

  private:

    int id;
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
  void DataInput<Content>::connect(DataPromiseMeta<Content>& p)
  {
    provider = &p;
    connect_timestamp = get_timestamp();
  }

  template <typename Content>
  const Content& DataInput<Content>::get_data()
  {
    if (provider == nullptr)
      throw "No provider for this input!";
    return provider->get_data();
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
