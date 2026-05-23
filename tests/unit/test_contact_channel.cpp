#include <catch2/catch_test_macros.hpp>
#include <cvengine/core/domain/contact_channel.hpp>
#include <cvengine/common/error.hpp>

TEST_CASE("ContactChannel domain entity validations", "[core][domain]") {
    using cvengine::core::domain::ContactChannel;
    using cvengine::common::ErrorCode;

    SECTION("Valid WhatsApp creation") {
        auto result = ContactChannel::create(
            ContactChannel::Type::WhatsApp,
            "WhatsApp",
            "51917672598"
        );
        REQUIRE(result.has_value());
        CHECK(result->type() == ContactChannel::Type::WhatsApp);
        CHECK(result->label() == "WhatsApp");
        CHECK(result->value() == "51917672598");
        CHECK(result->url() == "https://wa.me/51917672598");
    }

    SECTION("Invalid WhatsApp non-digits") {
        auto result = ContactChannel::create(
            ContactChannel::Type::WhatsApp,
            "WhatsApp",
            "+51917672598"
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidFormat);
    }

    SECTION("Valid LinkedIn creation") {
        auto result = ContactChannel::create(
            ContactChannel::Type::LinkedIn,
            "LinkedIn",
            "pedro-espinoza"
        );
        REQUIRE(result.has_value());
        CHECK(result->url() == "https://linkedin.com/in/pedro-espinoza");
    }

    SECTION("Valid GitHub creation") {
        auto result = ContactChannel::create(
            ContactChannel::Type::GitHub,
            "GitHub",
            "gvearde"
        );
        REQUIRE(result.has_value());
        CHECK(result->url() == "https://github.com/gvearde");
    }

    SECTION("Valid Website creation") {
        auto result = ContactChannel::create(
            ContactChannel::Type::Website,
            "Sitio Web",
            "https://miweb.com/cv"
        );
        REQUIRE(result.has_value());
        CHECK(result->url() == "https://miweb.com/cv");
    }

    SECTION("Invalid Website (missing scheme)") {
        auto result = ContactChannel::create(
            ContactChannel::Type::Website,
            "Sitio Web",
            "miweb.com/cv"
        );
        REQUIRE_FALSE(result.has_value());
        CHECK(result.error().code == ErrorCode::InvalidFormat);
    }

    SECTION("Empty fields validation") {
        auto result_label = ContactChannel::create(
            ContactChannel::Type::WhatsApp,
            "",
            "123"
        );
        CHECK(result_label.error().code == ErrorCode::EmptyRequiredField);

        auto result_value = ContactChannel::create(
            ContactChannel::Type::WhatsApp,
            "WA",
            ""
        );
        CHECK(result_value.error().code == ErrorCode::EmptyRequiredField);
    }

    SECTION("Type string conversions") {
        CHECK(ContactChannel::type_from_string("whatsapp") == ContactChannel::Type::WhatsApp);
        CHECK(ContactChannel::type_from_string("linkedin") == ContactChannel::Type::LinkedIn);
        CHECK(ContactChannel::type_from_string("github") == ContactChannel::Type::GitHub);
        CHECK(ContactChannel::type_from_string("website") == ContactChannel::Type::Website);
        CHECK(ContactChannel::type_from_string("twitter") == ContactChannel::Type::Twitter);
        CHECK(ContactChannel::type_from_string("unknown_type") == ContactChannel::Type::Other);

        CHECK(ContactChannel::type_to_string(ContactChannel::Type::WhatsApp) == "whatsapp");
        CHECK(ContactChannel::type_to_string(ContactChannel::Type::LinkedIn) == "linkedin");
        CHECK(ContactChannel::type_to_string(ContactChannel::Type::GitHub) == "github");
        CHECK(ContactChannel::type_to_string(ContactChannel::Type::Website) == "website");
        CHECK(ContactChannel::type_to_string(ContactChannel::Type::Twitter) == "twitter");
        CHECK(ContactChannel::type_to_string(ContactChannel::Type::Other) == "other");
    }
}
