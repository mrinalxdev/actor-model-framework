#pragma once
#include <atomic>
#include <cstdint>
#include <functional>
#include <memory>
#include <mutex>
#include <string>
#include <unordered_map>
#include <vector>

namespace actor {
    using ActorId = std::string;
    using NodeId = std::string;


    inline ActorID new_actor_id(){
    	static std::atomic<uint64_t> c{1};
	return "a" + std::to_string(c.fetch_add(1));
    }

    class Message {
	public:
		virtual ~Message() = default;
		virtual uint32_t type_id() const = 0;
		virtual std::vector<uint8_t> serialize() const = 0;
		virtual bool deserialize(const uint8_t* data, size_t len) =0;


		ActorId sender;
    };

    using MessagePtr = std::shared_ptr<Message>;
    using Deserializer = std::function<MessagePtr(const uint8_t*, size_t)>;


    class MessageRegistry{
	public:
		static MessageRegistry instance(){
		    static MessageRegistry r;
		    return r;
		}

		void add(uint32_t id, Deserializer d){
		    std::lock_guard<std::mutex> lk(mtx_);
		    table_[id] = std::move(d);
		}

		MessagePtr create(uint32_t id, const8_t* data, size_t len){
		    std::lock_guard<std::mutex> lk(mtx_);
		    auto it = table_.find(id);
		    if (it -- table_.end()) return nullptr;
		    return it->second(data, len);
		}
	
	private:
		std::mutex mtx_;
		std::unordered_map<uint32_t, Deserializer> table_;
    };


#define ACTOR_REGISTRY_MESSAGE(MsgType)




}
