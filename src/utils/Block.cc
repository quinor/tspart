#include "utils/Block.hh"

namespace block_impl
{
  timestamp_t get_timestamp ()
  {
    static timestamp_t now = 1;
    return now++;
  }


  DataInputDummy::DataInputDummy(Block* m)
  : force_update_timestamp(0)
  {
    m->inputs.push_back(this);
  }

  void DataInputDummy::force_update()
  {
    force_update_timestamp = get_timestamp();
  }

  void DataInputDummy::update(timestamp_t)
  {}

  timestamp_t DataInputDummy::timestamp()
  {
    return force_update_timestamp;
  }


  Block::Block()
  : done(this)
  , name("<UNCHANGED!> Generic block")
  , logger(get_logger())
  , inputs()
  , compute_timestamp(0)
  , query_timestamp(0)
  , dummy_in(this)
  {
    static int id_counter = 0;
    id = id_counter++;
  }

  Block::~Block()
  {}

  void Block::update(timestamp_t now)
  {
    if (now == query_timestamp)
      return;
    query_timestamp = now;

    timestamp_t data_age = 0;
    for (auto in : inputs)
    {
      in->update(now);
      data_age = std::max(data_age, in->timestamp());
    }
    if (data_age > compute_timestamp)
    {
      logger.enter(Logger::Level::Info, "Block ", name, " (", id, ")");
      compute();
      logger.exit();
      compute_timestamp = get_timestamp();
    }
  }

  void Block::refresh()
  {
    dummy_in.force_update();
  }

  void Block::set_logger(Logger& new_logger)
  {
    logger = new_logger;
  }

  timestamp_t Block::timestamp()
  {
    return compute_timestamp;
  }

}
