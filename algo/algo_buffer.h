#ifndef INCLUDED_ALGO_BUFFER
#define INCLUDED_ALGO_BUFFER

#ifndef INCLUDED_ALGO_BASICS
#include "algo_basics.h"
#endif

#ifndef INCLUDED_ALGO_TRAITS
#include "algo_traits.h"
#endif

#ifndef INCLUDED_ALGO_ITERATOR
#include "algo_iterator.h"
#endif

#ifndef INCLUDED_MEMORY
#include <memory>
#define INCLUDED_MEMORY
#endif

#ifndef INCLUDED_CSTDLIB
#include <cstdlib>
#define INCLUDED_CSTDLIB
#endif

namespace algo
{
    struct PointerAndSize
    {
        char* ptr ;
        ptrdiff_t size ;
    } ;
    
    
    
    struct BufferCalculation
    {
        template < typename T >
        static
        ALGO_INLINE
        T* calculateBegin ( PointerAndSize data )
        {
            ALGO_ASSERT ( ALGO_NULLPTR != data.ptr ) ;
            ALGO_ASSERT ( data.size > 0u ) ;
            
            const uintptr_t offset = reinterpret_cast < uintptr_t > ( data.ptr ) % uintptr_t ( ALGO_CALL::GetAlignmentOf < T >::value ) ;
            
            // Overflow check
            ALGO_ASSERT ( std::numeric_limits < uintptr_t >::max () - offset >= ALGO_CALL::SizeOf < T >::value ) ;
            
            if ( ALGO_LIKELIHOOD ( uintptr_t ( data.size ) >= ( ALGO_CALL::SizeOf < T >::value + offset ), true ) )
            {
                // Overflow check
                ALGO_ASSERT ( std::numeric_limits < ptrdiff_t >::max () >= offset ) ;
                
                return reinterpret_cast < T* > ( ALGO_CALL::advance ( data.ptr, ptrdiff_t ( offset ) ) ) ;
            }
            else
            {
                // Return null if it is not possible to fit in a single instance of type T into the buffer.
                return ALGO_NULLPTR ;
            }
        }
        
        template < typename T >
        static
        ALGO_INLINE
        T* calculateEnd ( T* const begin, PointerAndSize data )
        {
            ALGO_ASSERT ( ALGO_NULLPTR != data.ptr ) ;
            ALGO_ASSERT ( data.size > 0u ) ;
            
            if ( ALGO_LIKELIHOOD ( ALGO_NULLPTR == begin, false ) )
            {
                return ALGO_NULLPTR ;
            }
            else
            {
                ptrdiff_t const offset = ALGO_CALL::distance ( data.ptr, reinterpret_cast < char* >( begin ) ) ;
                ALGO_ASSERT ( offset >= 0 ) ;
                
                // asertion is equivalent to ( data.size - offset ) / ALGO_CALL::SizeOf < T >::value but without the divide
                // and the dangerous subtract
                ALGO_ASSERT ( uintptr_t ( data.size ) >= ( ALGO_CALL::SizeOf < T >::value + offset ) ) ;
                
                // Overflow check
                ALGO_ASSERT ( ( data.size - offset ) <= uintptr_t ( std::numeric_limits < ptrdiff_t >::max () ) ) ;
                
                return ALGO_CALL::advance ( begin, ptrdiff_t ( data.size - offset ) / ALGO_CALL::SizeOf < T >::value ) ;
            }
        }
    } ;
    
    // Provides storage, does not manage object lifetimes, nor is it coupled to the underlying object type
    template < ptrdiff_t Size >
    struct StackBuffer
    {
        // No need to make it private and remove its pod-ness
        char d_buff [ Size ] ;
        
        template < typename T >
        ALGO_INLINE
        T* begin ()
        {
            return ALGO_CALL::BufferCalculation::template calculateBegin < T > ( { &this->d_buff[0], Size } ) ;
        }
        
        template < typename T >
        ALGO_INLINE
        T* end ()
        {
            return ALGO_CALL::BufferCalculation::calculateEnd ( this->template begin < T > (), { &this->d_buff[0], Size } ) ;
        }
    } ;
    
    
    
    struct MallocFreeProtocol
    {
        ALGO_INLINE
        static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
        {
            void* const returnValue = malloc ( size ) ;
            
            if ( ALGO_LIKELIHOOD ( returnValue, true ) )
            {
                return { reinterpret_cast < char* > ( returnValue ), size } ;
            }
            else
            {
                throw std::bad_alloc () ;
            }
        }
        
        ALGO_INLINE
        static void deallocate ( ALGO_CALL::PointerAndSize data )
        {
            if ( ALGO_LIKELIHOOD ( data.ptr, true ) )
            {
                free ( data.ptr ) ;
            }
        }
    } ;
    
    
    
    struct CallocFreeProtocol : MallocFreeProtocol
    {
        ALGO_INLINE
        static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
        {
            void* const returnValue = calloc ( size, 1u ) ;
            
            if ( ALGO_LIKELIHOOD ( returnValue, true ) )
            {
                return { reinterpret_cast < char* > ( returnValue ), size } ;
            }
            else
            {
                throw std::bad_alloc () ;
            }
        }
    } ;
    
    
    
    struct NewDeleteProtocol
    {
        ALGO_INLINE
        static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
        {
            return { new char [ size ], size } ;
        }
        
        ALGO_INLINE
        static void deallocate ( ALGO_CALL::PointerAndSize data )
        {
            if ( ALGO_LIKELIHOOD ( data.ptr, true ) )
            {
                delete [] ( data.ptr ) ;
            }
        }
    } ;
    
    // ZeroedNewDeleteProtocol uses fill
    
    struct StlTemporaryBufferProtocol
    {
        ALGO_INLINE
        static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
        {
            const std::pair< char*, std::ptrdiff_t > returnValue = std::get_temporary_buffer < char > ( size ) ;
            if ( ALGO_LIKELIHOOD ( returnValue.first, true ) )
            {
                return { returnValue.first, returnValue.second } ;
            }
            else
            {
                throw std::bad_alloc () ;
            }
        }
        
        ALGO_INLINE
        static void deallocate ( ALGO_CALL::PointerAndSize data )
        {
            if ( ALGO_LIKELIHOOD ( data.ptr, true ) )
            {
                std::return_temporary_buffer ( data.ptr ) ;
            }
        }
    } ;
    
    // ZeroedStlTemporaryBufferProtocol uses fill
    
    template < typename AllocatorProtocol >
    class AllocatingBuffer
    {
    public:
        ALGO_INLINE
        explicit AllocatingBuffer ( const ptrdiff_t size )
        : d_data ( this->allocate ( size ) )
        {}
        
        ALGO_INLINE
        ~AllocatingBuffer ()
        {
            this->deallocate ( this->d_data ) ;
            ALGO_SAFE_MODE ( this->d_data.ptr = ALGO_NULLPTR ) ;
            ALGO_SAFE_MODE ( this->d_data.size = 0u ) ;
        }
        
        template < typename T >
        ALGO_INLINE
        T* begin ()
        {
            return ALGO_CALL::BufferCalculation::template calculateBegin < T > ( this->d_data ) ;
        }
        
        template < typename T >
        ALGO_INLINE
        T* end ()
        {
            return ALGO_CALL::BufferCalculation::calculateEnd ( this->template begin < T > (), this->d_data ) ;
        }
        
    private:
        AllocatingBuffer ( AllocatingBuffer const& ) ;
        AllocatingBuffer& operator= ( AllocatingBuffer const& ) ;
        
        ALGO_INLINE
        static ALGO_CALL::PointerAndSize allocate ( const ptrdiff_t size )
        {
            ALGO_ASSERT ( size > 0 ) ;
            return AllocatorProtocol::allocate ( size ) ;
        }
        
        ALGO_INLINE
        static void deallocate ( ALGO_CALL::PointerAndSize data )
        {
            AllocatorProtocol::deallocate ( data ) ;
        }
        
        PointerAndSize d_data ;
    } ;
    
    
    
    template < typename T, bool destructionLikely = false ALGO_COMMA_ENABLE_IF_PARAM >
    struct ObjectProctor
    {
        ALGO_INLINE
        explicit ObjectProctor ( T* obj )
        : d_obj ( obj )
        {}
        
        ALGO_INLINE
        explicit ObjectProctor ( T& obj )
        : d_obj ( &obj )
        {}
        
        ALGO_INLINE
        ~ObjectProctor ()
        {
            if ( ALGO_LIKELIHOOD ( this->d_obj, destructionLikely ) )
            {
                this->destroyObject () ;
            }
        }
        
        ALGO_INLINE
        void disarm ()
        {
            this->d_obj = ALGO_NULLPTR ;
        }
    private:
        
        ObjectProctor ( ObjectProctor const& x ) ;
        ObjectProctor& operator= ( ObjectProctor const& ) ;
        
        // non-inline, for unusual case of having to do something
        void destroyObject ()
        {
            T* const obj = this->d_obj ;
            ALGO_SAFE_MODE ( this->d_obj = ALGO_NULLPTR ) ;
            ALGO_CALL::destroyPointed ( obj ) ;
        }
        
        T* d_obj ;
    } ;
    
    
    // Optimisation for when destruction is not required
    template < typename T, bool destructionLikely >
    struct ObjectProctor < T, destructionLikely,
    typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < T >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        explicit ObjectProctor ( T* )
        {}
        
        ALGO_INLINE
        explicit ObjectProctor ( T& )
        {}
        
        // No need for a destructor
        
        ALGO_INLINE
        void disarm ()
        {}
    private:
        
        ObjectProctor ( ObjectProctor const& x ) ;
        ObjectProctor& operator= ( ObjectProctor const& ) ;
    } ;
    
    
    
    template < typename Iter, bool destructionLikely = false ALGO_COMMA_ENABLE_IF_PARAM >
    struct IterProctor
    {
        ALGO_INLINE
        explicit IterProctor ( Iter x )
        : d_start ( x )
        , d_end   ( x )
        {}
        
        ALGO_INLINE
        ~IterProctor ()
        {
            if ( ALGO_LIKELIHOOD ( this->d_start != this->d_end, destructionLikely ) )
            {
                this->destroyExtent () ;
            }
        }
        
        ALGO_INLINE
        void incrementStart ()
        {
            // Can't increment start past end
            ALGO_ASSERT ( this->d_start != this->d_end ) ;
            ALGO_CALL::successor ( this->d_start, ALGO_CALL::InPlace () ) ;
        }
        
        ALGO_INLINE
        void incrementEnd ()
        {
            ALGO_CALL::successor ( this->d_end, ALGO_CALL::InPlace () ) ;
        }
        
    private:
        IterProctor ( IterProctor const& ) ;
        IterProctor& operator= ( IterProctor const& ) ;
        
        // Keep non-inline ... code is unlikely to be executed
        void destroyExtent ()
        {
            while ( this->d_start != this->d_end )
            {
                ALGO_CALL::destroyPointed ( this->d_start ) ; // Throwing on destruction is undefined behaviour ...
                ALGO_CALL::successor ( this->d_start, ALGO_CALL::InPlace () ) ;
            }
        }
        
        Iter d_start ;
        Iter d_end ;
    } ;
    
    
    
    // Optimisation for when destruction is not required
    template < typename Iter, bool destructionLikely >
    struct IterProctor < Iter, destructionLikely,
    typename std::enable_if < ALGO_CALL::IsTriviallyDestructible < typename std::iterator_traits < Iter >::value_type >::value, ALGO_ENABLE_IF_PARAM_DEFAULT>::type >
    {
        ALGO_INLINE
        explicit IterProctor ( Iter )
        {}
        
        // No need for a destructor
        
        ALGO_INLINE
        void incrementStart ()
        {}
        
        ALGO_INLINE
        void incrementEnd ()
        {}
        
    private:
        IterProctor ( IterProctor const& ) ;
        IterProctor& operator= ( IterProctor const& ) ;
    } ;
    
    
    // Acts as a simple wrapper to the more general IterProctor, to save clients having to type T* rather than T.
    template < typename T, bool destructionLikely = false ALGO_COMMA_ENABLE_IF_PARAM >
    struct BufferProctor : ALGO_CALL::IterProctor < T*, destructionLikely >
    {
        typedef ALGO_CALL::IterProctor < T*, destructionLikely > ParentType ;
        
        ALGO_INLINE
        explicit BufferProctor ( T* x )
        : ParentType ( x )
        {}
    } ;
    
} // namespace algo

#endif
