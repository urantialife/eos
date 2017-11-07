/**
 *  @file
 *  @copyright defined in eos/LICENSE.txt
 */
#pragma once
#include <fc/io/raw.hpp>

#include <eos/chain/transaction.hpp>
#include <fc/uint128.hpp>

#include <boost/multi_index/hashed_index.hpp>
#include <boost/multi_index/mem_fun.hpp>

#include "multi_index_includes.hpp"

namespace eosio { namespace chain {
   using boost::multi_index_container;
   using namespace boost::multi_index;
   /**
    * The purpose of this object is to store transactions generated by processing the
    * transactions included in the chain.  These transactions should be treated like
    * authentic/valid SignedTransactions for the purposes of scheduling transactions
    * in to new blocks
    */
   class generated_transaction_object : public chainbase::object<generated_transaction_object_type, generated_transaction_object>
   {
         OBJECT_CTOR(generated_transaction_object)

         enum status_type {
            PENDING = 0,
            PROCESSED
         };


         id_type                       id;
         GeneratedTransaction          trx;
         status_type                   status;
         
         time_point_sec get_expiration()const { return trx.expiration; }
         generated_transaction_id_type get_id() const { return trx.id; }

         struct by_trx_id;
         struct by_expiration;
         struct by_status;
   };

   using generated_transaction_multi_index = chainbase::shared_multi_index_container<
      generated_transaction_object,
      indexed_by<
         ordered_unique<tag<by_id>, BOOST_MULTI_INDEX_MEMBER(generated_transaction_object, generated_transaction_object::id_type, id)>,
         hashed_unique<tag<generated_transaction_object::by_trx_id>, const_mem_fun<generated_transaction_object, generated_transaction_id_type, &generated_transaction_object::get_id>>,
         ordered_non_unique<tag<generated_transaction_object::by_expiration>, const_mem_fun<generated_transaction_object, time_point_sec, &generated_transaction_object::get_expiration>>,
         ordered_non_unique<tag<generated_transaction_object::by_status>, BOOST_MULTI_INDEX_MEMBER(generated_transaction_object, generated_transaction_object::status_type, status)>
      >
   >;

   typedef chainbase::generic_index<generated_transaction_multi_index> generated_transaction_index;
} }

CHAINBASE_SET_INDEX_TYPE(eosio::chain::generated_transaction_object, eosio::chain::generated_transaction_multi_index)

FC_REFLECT( eosio::chain::generated_transaction_object, (trx) )
