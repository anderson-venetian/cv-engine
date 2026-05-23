#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/contact.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("Contact domain entity validations", "[core][domain]") {
    using cvengine::core::domain::Contact;
    using cvengine::core::domain::ContactChannel;
    using cvengine::common::ErrorCode;

    SECTION("Valid contact creation") {
        auto result = Contact::create(
            "Pedro Espinoza",
            "Lima, Perú",
            "917672598",
            "pedro@istp.edu.pe"
        );
        REQUIRE(result.has_value());
        CHECK(result->name() == "Pedro Espinoza");
        CHECK(result->location() == "Lima, Perú");
        CHECK(result->phone() == "917672598");
        CHECK(result->email() == "pedro@istp.edu.pe");
        CHECK(result->extra_channels().empty());
    }

    SECTION("Valid contact with channels") {
        auto chan_r = ContactChannel::create(ContactChannel::Type::GitHub, "Git", "gvearde");
        REQUIRE(chan_r.has_value());

        std::vector<ContactChannel> channels { *chan_r };
        auto result = Contact::create(
            "Pedro Espinoza",
            "Lima, Perú",
            "917672598",
            "pedro@istp.edu.pe",
            channels
        );
        REQUIRE(result.has_value());
        CHECK(result->extra_channels().size() == 1);
        CHECK(result->extra_channels().front().value() == "gvearde");
    }

    SECTION("Fail on empty name") {
        auto result = Contact::create(
            "",
            "Lima",
            "123",
            "email@test.com"
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty location") {
        auto result = Contact::create(
            "Name",
            "",
            "123",
            "email@test.com"
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on empty email") {
        auto result = Contact::create(
            "Name",
            "Lima",
            "123",
            ""
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Fail on invalid email format") {
        auto result = Contact::create(
            "Name",
            "Lima",
            "123",
            "invalid_email_format"
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidFormat);
    }
}
